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
	,playerMaterial()
	,m_chanceOfHit( 0.2f )
	
{
	health = defaultHealth;

	primaryGun = CreateDefaultSubobject<UChildActorComponent>( TEXT( "PrimaryGun" ) );
	pistol = CreateDefaultSubobject<UChildActorComponent>( TEXT( "Pistol" ) );
	m_weaponInventory.Add( primaryGun );
	m_weaponInventory.Add( pistol );


	primaryGun->SetupAttachment( GetMesh(), weaponSocket );
	bulletComp->SetupAttachment( primaryGun, muzzleSocket );

	//static ConstructorHelpers::FObjectFinder<UMaterial>
	//	material( TEXT( "Material'/Game/ProjectRelic/StaticMeshes/Player/AlienSoldier/Ch44_Body.Ch44_Body'" ) );


	//if( material.Object != NULL )
	//{
	//	playerMaterial = ( UMaterial* ) material.Object;
	//}
	//m_playerMaterial = UMaterialInstanceDynamic::Create( TheMaterial, this );
	// 
	
}

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Start player off as crouching
	//Crouch();

	gunComp->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), weaponSocket );

	EquipGun( m_weaponInventory );

	// Set texture of the player
	m_invisibility = false;

	// Create dynamic material
	m_material = GetMesh()->GetMaterial( 0 );
	m_material2 = GetMesh()->GetMaterial( 1 );
	m_dynamicMaterial = UMaterialInstanceDynamic::Create( m_material, this );
	GetMesh()->SetMaterial( 0, m_dynamicMaterial );
	m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );
	m_dynamicMaterial->SetVectorParameterValue( TEXT( "Colour" ), FLinearColor::Red );
	
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


	//PlayerInputComponent->BindAction( "CoverButton", IE_Pressed, this, &ACPP_CharacterManager::StartCover );
}

void ACPP_PlayerManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	userInterfaceDelegate();

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


	if( m_aimingIn == true )
	{
		m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( true );
	}
	else
	{
		m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( false );
	}

	if( m_invisibility )
	{
		//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, ( TEXT( "INVISIBILITY ON" ) ) );

		// Change texture
		//m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 50 );

		GetMesh()->SetMaterial( 0, playerMaterial );
		GetMesh()->SetMaterial( 1, playerMaterial );

	}
	if( !m_invisibility )
	{
		//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, ( TEXT( "INVISIBILITY OFF" ) ) );

		// Back to original texture
		m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );

		GetMesh()->SetMaterial( 0, m_material );
		GetMesh()->SetMaterial( 1, m_material2 );
	}
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
	TakeAttack(); // DELETE THIS ----------------------------------------------------

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
	m_ammoCount += ammo;

	
	//GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Purple, FString::SanitizeFloat( m_ammoCount ) );
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

	// Attach to gun socket
	WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket );
	
	// If AR is chosen
	if( m_currentlyEquipped == 0 )
	{
		// Set bools
		m_assaultRifle = true;
		m_pistol = false;
	}
	// If pistol is chosen
	if( m_currentlyEquipped == 1 )
	{
		// Set bools
		m_assaultRifle = false;
		m_pistol = true;
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
	return m_hitmarker;
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
	userInterfaceD.Broadcast( health / 100.0f, m_invisibilityPercent / 100.0f, m_ammoCount, m_reserveAmmo );
}


