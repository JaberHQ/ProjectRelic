// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CPP_AIController.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CPP_AIManager.h"



ACPP_PlayerManager::ACPP_PlayerManager()
	:m_canTakedown( true )
	,takedownTraceDistance( 250.0f )
	,defaultHealth( 100.0f )
	,health( 100.0f )
	,m_shotDamage( 15.0f )
	,m_animPosition( 40.0f )
	,m_animCompletion( 5.0f )
	,m_invisibility( false )
	,m_invisibilityPercent( 100.0f )
	,m_invisiblityTimer()
	,m_currentlyEquipped( 0 )
	,m_weaponInventory()
	,invisibleMaterial()
	,m_chanceOfHit( 0.2f )
	,m_pistolSocket(TEXT( "PistolSocket" ) )
	,m_pistolMuzzleSocket( TEXT(" PistolMuzzleSocket" ) )
	,callEnemy()
	,m_hitmarkerActive( false )
	,m_hitmarkerTimer()
{
	health = defaultHealth;

	primaryGun = CreateDefaultSubobject<UChildActorComponent>( TEXT( "PrimaryGun" ) );
	pistol = CreateDefaultSubobject<UChildActorComponent>( TEXT( "Pistol" ) );
	m_weaponInventory.Add( primaryGun );
	m_weaponInventory.Add( pistol );
	m_weaponInventory.Add( throwable );

	pistol->SetupAttachment( GetMesh(), m_pistolSocket );
	
}

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	gunComp->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), weaponSocket );
	primaryGun->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket );
	bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, muzzleSocket );
	throwable->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket );
	m_throwSceneComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket);

	EquipGun( m_weaponInventory );

	// Set texture of the player
	m_invisibility = false;

	// Create dynamic material
	m_material = GetMesh()->GetMaterial( 0 );
	m_material1 = GetMesh()->GetMaterial( 1 );
	m_material2 = GetMesh()->GetMaterial( 2 );
	m_material3 = GetMesh()->GetMaterial( 3 );
	m_material4 = GetMesh()->GetMaterial( 4 );
	m_material5 = GetMesh()->GetMaterial( 5 );
	m_material6 = GetMesh()->GetMaterial( 6 );
	m_material7 = GetMesh()->GetMaterial( 7 );
	m_material8 = GetMesh()->GetMaterial( 8 );
	m_material9 = GetMesh()->GetMaterial( 9 );
	m_material10 = GetMesh()->GetMaterial( 10 );

	m_dynamicMaterial = UMaterialInstanceDynamic::Create( m_material, this );
	GetMesh()->SetMaterial( 0, m_dynamicMaterial );
	m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );
	m_dynamicMaterial->SetVectorParameterValue( TEXT( "Colour" ), FLinearColor::Red );
	
	//pistol->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), m_pistolSocket );
}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( "ChangeWeapons", this, &ACPP_PlayerManager::ChangeWeapons );

	PlayerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &ACPP_PlayerManager::Takedown );
	PlayerInputComponent->BindAction( "PowerUp", IE_Pressed, this, &ACPP_PlayerManager::Invisibility );
	PlayerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACPP_CharacterManager::StartShooting );
	PlayerInputComponent->BindAction( "Shoot", IE_Released, this, &ACPP_CharacterManager::StopShooting );
	PlayerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACPP_CharacterManager::StartAim );
	PlayerInputComponent->BindAction( "Aim", IE_Released, this, &ACPP_CharacterManager::StopAim );
	PlayerInputComponent->BindAction( "CoverButton", IE_Pressed, this, &ACPP_CharacterManager::WallTrace );
	PlayerInputComponent->BindAction( "DistractEnemy", IE_Pressed, this, &ACPP_PlayerManager::DistractEnemy );



	//PlayerInputComponent->BindAction( "CoverButton", IE_Pressed, this, &ACPP_CharacterManager::StartCover );
}

void ACPP_PlayerManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	userInterfaceDelegate();

	InvisibilityTick( DeltaTime );

	// If player is aiming in, show weapon
	m_aimingIn ? m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( true ) : m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( false );

	if( m_ammoAR == 0 || m_ammoPistol == 0 )
	{
		m_isShooting = false;
	}

	AmmoTick();

	if( m_hitmarkerActive )
	{
		GetWorld()->GetTimerManager().SetTimer( m_hitmarkerTimer, this, &ACPP_PlayerManager::HitmarkerFinished, 1.0f, true );
	}
}

void ACPP_PlayerManager::InvisibilityTick( float DeltaTime )
{
	InvisibilityEvaluation( DeltaTime );

	InvisibilityMaterial();
}
void ACPP_PlayerManager::InvisibilityEvaluation( float DeltaTime )
{
	if( m_invisibilityPercent > 0.0f && m_invisibility == true )
	{
		GetWorld()->GetTimerManager().SetTimer( m_invisiblityTimer, this, &ACPP_PlayerManager::InvisibilityFinished, 1.0f, true );
		m_invisibilityPercent -= ( DeltaTime * 25.0f );
	}

	if( m_invisibilityPercent < 100.0f && m_invisibility == false || m_invisibilityPercent == 0 )
	{
		m_invisibilityPercent += ( DeltaTime * 10.0f );
	}

	if( m_invisibilityPercent == 0 )
	{
		m_invisibilityPercent += ( DeltaTime * 10.0f );
	}
}

void ACPP_PlayerManager::InvisibilityMaterial()
{
	// If player is invisible
	if( m_invisibility )
	{
		for( int i = 0; i < 10; i++ )
		{
			GetMesh()->SetMaterial( i, invisibleMaterial );
		}
		/*GetMesh()->SetMaterial( 0, invisibleMaterial );
		GetMesh()->SetMaterial( 1, invisibleMaterial );
		GetMesh()->SetMaterial( 2, invisibleMaterial );
		GetMesh()->SetMaterial( 3, invisibleMaterial );
		GetMesh()->SetMaterial( 4, invisibleMaterial );
		GetMesh()->SetMaterial( 5, invisibleMaterial );
		GetMesh()->SetMaterial( 6, invisibleMaterial );
		GetMesh()->SetMaterial( 7, invisibleMaterial );
		GetMesh()->SetMaterial( 8, invisibleMaterial );
		GetMesh()->SetMaterial( 9, invisibleMaterial );
		GetMesh()->SetMaterial( 10, invisibleMaterial );*/

	}
	else
	{
		// Back to original texture
		m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );
		/*for( int i = 0; i < 10; i++ )
		{
			GetMesh()->SetMaterial( i, m_material );
		}*/
		GetMesh()->SetMaterial( 0, m_material );
		GetMesh()->SetMaterial( 1, m_material1 );
		GetMesh()->SetMaterial( 2, m_material2 );
		GetMesh()->SetMaterial( 3, m_material3 );
		GetMesh()->SetMaterial( 4, m_material4 );
		GetMesh()->SetMaterial( 5, m_material5 );
		GetMesh()->SetMaterial( 6, m_material6 );
		GetMesh()->SetMaterial( 7, m_material7 );
		GetMesh()->SetMaterial( 8, m_material8 );
		GetMesh()->SetMaterial( 9, m_material9 );
		GetMesh()->SetMaterial( 10, m_material10 );

	}
}
void ACPP_PlayerManager::AmmoTick()
{
	if( m_assaultRifle )
	{
		// If there is reserve ammo available
		if( m_ammoAR > m_fullMagAR )
		{
			m_reserveAR = m_ammoAR - m_fullMagAR;
			m_ammoAR = m_fullMagAR;
		}

		if( m_isShooting )
		{
		}

		// If there is no ammo in the main tank left
		if( m_ammoAR == 0 )
		{
			m_isShooting = false;

			// If there is enough reserve for a full reload
			if( m_reserveAR >= m_fullMagAR )
			{
				m_ammoAR = m_fullMagAR;

				m_reserveAR -= m_fullMagAR;
			}

			// If there is not enough reserve for a full reload but enough for a partial reload
			if( m_reserveAR < m_fullMagAR && m_reserveAR > 0 )
			{
				m_ammoAR = m_reserveAR;

				m_reserveAR = 0;
			}
		}
	}

	if( m_pistol )
	{
		// If there is reserve ammo available
		if( m_ammoPistol > m_fullMagPistol )
		{
			m_reservePistol = m_ammoPistol - m_fullMagPistol;
			m_ammoPistol = m_fullMagPistol;
		}

		if( m_isShooting )
		{
		}

		// If there is no ammo in the main tank left
		if( m_ammoPistol == 0 )
		{
			m_isShooting = false;

			// If there is enough reserve for a full reload
			if( m_reservePistol >= m_fullMagPistol )
			{
				m_ammoPistol = m_fullMagPistol;

				m_reservePistol -= m_fullMagPistol;
			}

			// If there is not enough reserve for a full reload but enough for a partial reload
			if( m_reservePistol < m_fullMagPistol && m_reservePistol > 0 )
			{
				m_ammoPistol = m_reservePistol;

				m_reservePistol = 0;
			}
		}
	}
}
void ACPP_PlayerManager::AmmoEvaluation( int ammoCount, int reserveCount, int fullMag )
{
	// If there is reserve ammo available
	if( ammoCount > fullMag )
	{
		reserveCount = ammoCount - fullMag;
		ammoCount = fullMag;
	}


	// If there is no ammo in the main tank left
	if( ammoCount == 0 )
	{
		m_isShooting = false;

		// If there is enough reserve for a full reload
		if( reserveCount >= fullMag )
		{
			ammoCount = fullMag;

			reserveCount -= fullMag;
		}

		// If there is not enough reserve for a full reload but enough for a partial reload
		if( reserveCount < fullMag && reserveCount > 0 )
		{
			ammoCount = reserveCount;

			reserveCount = 0;
		}
	}
}
void ACPP_PlayerManager::HitmarkerFinished()
{
	m_hitmarkerActive = false;
}
void ACPP_PlayerManager::SetCanTakedown( bool canTakedown )
{	
	m_canTakedown = canTakedown;	
}

bool ACPP_PlayerManager::GetCanTakedown()
{
	return m_canTakedown;
}

void ACPP_PlayerManager::Takedown()
{
	if( m_canTakedown )
	{
		TraceForwardImplementation();
	}
}

void ACPP_PlayerManager::Invisibility()
{
	// If invisiblity bar is above 0
	if( m_invisibilityPercent > 0.0f )
	{
		m_invisibility = !m_invisibility;
	}
}

bool ACPP_PlayerManager::GetInvisibilityStatus()
{
	return m_invisibility;
}

void ACPP_PlayerManager::InvisibilityFinished()
{
	m_invisibility = false;
}

void ACPP_PlayerManager::Respawn()
{
	PlayAnimMontage( deadAnim );

	health = defaultHealth;
	//SetActorLocation( FVector( 1552.347534, 258.646729, 1772.000000 ) );

}

void ACPP_PlayerManager::IncreaseAmmoCount( int ammo )
{
	m_ammoAR += 10;
	m_ammoPistol += 5;	
}

void ACPP_PlayerManager::EquipGun( TArray<UChildActorComponent*> WeaponInventory )
{
	// Turn all elements invisible
	for( int i = 0; i < WeaponInventory.Num(); i++ )
	{
		// Set Visibility to false
		WeaponInventory[ i ]->SetVisibility( false );
	}
	
	// Set the current weapon to be visibile
	WeaponInventory[ m_currentlyEquipped ]->SetVisibility( true );

	// If AR is chosen
	if( m_currentlyEquipped == 0 )
	{
		// Set bools
		m_assaultRifle = true;
		m_pistol = false;
		m_throwable = false;

		// Attach to gun socket
		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket );
		bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, muzzleSocket );

	}
	// If pistol is chosen
	if( m_currentlyEquipped == 1 )
	{
		// Set bools
		m_assaultRifle = false;
		m_pistol = true;
		m_throwable = false;

		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolSocket );
		bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolMuzzleSocket );

	}
	if( m_currentlyEquipped == 2 )
	{
		// Set bools
		m_assaultRifle = false;
		m_pistol = false;
		m_throwable = true;

		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket );

	}
}

void ACPP_PlayerManager::ChangeWeapons( float inputAxis )
{
	int value = UKismetMathLibrary::FTrunc( inputAxis );
	int changeWeaponValue = value + m_currentlyEquipped;

	if( m_weaponInventory.IsValidIndex( changeWeaponValue ) )
	{
		m_currentlyEquipped = changeWeaponValue;
	}
	else 
	{
		if( changeWeaponValue < 0 )
		{
			m_currentlyEquipped = m_weaponInventory.Num() - 1;
		}
		else
		{
			m_currentlyEquipped = 0;
		}
	}

	EquipGun( m_weaponInventory );

}

bool ACPP_PlayerManager::GetHitmarkerActive()
{
	return m_hitmarkerActive;
}

bool ACPP_PlayerManager::GetAssaultRifle()
{
	return m_assaultRifle;
}

bool ACPP_PlayerManager::GetPistol()
{
	return m_pistol;
}

bool ACPP_PlayerManager::GetThrowable()
{
	return m_throwable;
}

void ACPP_PlayerManager::SetHitmarkerActive( bool hitmarkerActive )
{
	m_hitmarkerActive = hitmarkerActive;
}

void ACPP_PlayerManager::DistractEnemy()
{
	if( callEnemy )
	{
		FVector const location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), callEnemy, location, 0.4f );
		UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 1.0f, this, 0.0f, noiseTag );
	}
}

void ACPP_PlayerManager::TraceForwardImplementation()
{
	FVector location; // Location
	FRotator rotation; // Rotation
	FHitResult hit; // Hit
	FCollisionQueryParams traceParams; // Trace parameters

	GetController()->GetPlayerViewPoint( location, rotation );

	// Set start and end
	FVector start = location;
	FVector end = ( GetActorLocation() + ( GetActorForwardVector() * takedownTraceDistance ) );

	// Line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_WorldDynamic, traceParams );

	// Draw a line for debug
	//DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 5.0f );

	// If line has hit
	if( bHit )
	{
		// Box where collision has occured
		DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );

		// Get AI controller
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( hit.GetActor() );
		if( managerAI )
		{
			// Debug
			//GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, hit.GetActor()->GetName() );
			
			// --- Spring arm stuff ---

			// Disable character movement
			GetCharacterMovement()->DisableMovement();

			// Disable Player input
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			DisableInput( playerController );

			// Set bool
			m_canTakedown = false;

			// Set actor transform
			SetActorRotation( managerAI->GetActorRotation() ); // Meant to be new transform rotation, not actor rotation
			SetActorLocation( ( managerAI->GetActorLocation() ) + ( managerAI->GetActorForwardVector() * -m_animPosition ) );
			
			if( animTakedown )
			{
				// Play animation
				PlayAnimMontage( animTakedown );
				
				// Takedown AI
				managerAI->Takedown();

				// Delay 
				FTimerHandle delayTimer;
				GetWorld()->GetTimerManager().SetTimer( delayTimer, this, &ACPP_PlayerManager::AnimationExecuted, m_animCompletion, false );		
			}
		}
	}
}

void ACPP_PlayerManager::AnimationExecuted()
{
	// Return character back to position
	//SetActorLocation( GetActorLocation() + GetActorForwardVector() * m_animPosition );

	// Enable Player input
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	EnableInput( playerController );

	// Re-enable movement
	GetCharacterMovement()->SetMovementMode( EMovementMode::MOVE_Walking );

	// --- Move back camera ---

	// Player can takedown AI again
	m_canTakedown = true;
}

void ACPP_PlayerManager::TakeAttack()
{
	// Random hit
	float randomHit = UKismetMathLibrary::RandomFloat();

	if( randomHit < m_chanceOfHit )
	{
		// If health remains, decrease health else pronounce Player as dead
		health > 0.0f ? health -= m_shotDamage : Respawn();
	}

	// Debug
	//FString healthDebug = FString::SanitizeFloat( health );
	//GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, FString::SanitizeFloat( chanceOfHit ) );
	
}

void ACPP_PlayerManager::userInterfaceDelegate()
{
	if( m_assaultRifle )
	{
		userInterfaceD.Broadcast( health / 100.0f, m_invisibilityPercent / 100.0f, m_ammoAR, m_reserveAR, m_assaultRifle, m_pistol, m_throwable );
	}
	if( m_pistol )
	{
		userInterfaceD.Broadcast( health / 100.0f, m_invisibilityPercent / 100.0f, m_ammoPistol, m_reservePistol, m_assaultRifle, m_pistol, m_throwable );

	}
	if( m_throwable )
	{
		userInterfaceD.Broadcast( health / 100.0f, m_invisibilityPercent / 100.0f, m_throwableAmount, 0, m_assaultRifle, m_pistol, m_throwable );

	}
}


