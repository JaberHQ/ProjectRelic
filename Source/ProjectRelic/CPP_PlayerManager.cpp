#define COLLISION_COVER ECC_GameTraceChannel2


#include "CPP_PlayerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CPP_AIController.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CPP_AIManager.h"
#include "CPP_Settings.h"

ACPP_PlayerManager::ACPP_PlayerManager()
	:m_canTakedown( false )
	,takedownTraceDistance( 250.0f )
	,health( Settings::k_defaultHealth )
	,m_shotDamage( 15.0f )
	,m_animPosition( 40.0f )
	,m_animCompletion( 5.0f )
	,m_invisibility( false )
	,m_invisibilityPercent( Settings::k_defaultInvisibility )
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
	,m_invisibilityFull( 100.0f )
	,m_invisibilityTimeMultiplier( 10.0f )
	,m_invisibilityTimeDrain( 25.0f )
	,m_isInCover()
	,footstepsSFX()
	,takedownAvailable( false )
	,m_pPlayerController( nullptr )
{
	// Create components
	primaryGun = CreateDefaultSubobject<UChildActorComponent>( TEXT( "PrimaryGun" ) );
	pistol = CreateDefaultSubobject<UChildActorComponent>( TEXT( "Pistol" ) );
	
	// Add child actors to array
	m_weaponInventory.Add( primaryGun );
	m_weaponInventory.Add( pistol );
	m_weaponInventory.Add( throwable );

	// Attach pistol
	pistol->SetupAttachment( GetMesh(), m_pistolSocket );
	throwable->SetupAttachment( GetMesh(), m_throwSocket );
}

ACPP_PlayerManager::~ACPP_PlayerManager()
{
	//APlayerController* m_pPlayerController = nullptr;
	//delete m_pPlayerController;
	m_pPlayerController = nullptr;

	
	
}

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Attach components to sockets
	gunComp->AttachToComponent( GetMesh(), FAttachmentTransformRules( EAttachmentRule::SnapToTarget, true ), weaponSocket );
	primaryGun->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, weaponSocket );
	bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, muzzleSocket );
	throwable->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket );
	m_throwSceneComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket);

	// Equip weapon
	EquipGun( m_weaponInventory );

	// Set texture of the player
	m_invisibility = false;

	// Create dynamic material

	for( int i = 0; i < GetMesh()->GetNumMaterials(); i++ )
	{
		m_material.Add( GetMesh()->GetMaterial( i ) );
	}

	// Create dynamic material
	//m_dynamicMaterial = UMaterialInstanceDynamic::Create( m_material, this );

	// Set dynamic material
	//GetMesh()->SetMaterial( 0, m_dynamicMaterial );
	//m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );
	//m_dynamicMaterial->SetVectorParameterValue( TEXT( "Colour" ), FLinearColor::Red );
	
	// Set health to full
	//health = defaultHealth;
}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	PrimaryActorTick.bCanEverTick = true;


	PlayerInputComponent->BindAxis( "MoveForward", this, &ACPP_PlayerManager::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACPP_PlayerManager::MoveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &ACPP_PlayerManager::Turn );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	//PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	//PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );
	//PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACPP_CharacterManager::BeginCrouch );
	//PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &ACPP_CharacterManager::EndCrouch );
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACPP_PlayerManager::BeginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &ACPP_PlayerManager::EndSprint );

	PlayerInputComponent->BindAxis( "ChangeWeapons", this, &ACPP_PlayerManager::ChangeWeapons );

	PlayerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &ACPP_PlayerManager::Takedown );
	PlayerInputComponent->BindAction( "PowerUp", IE_Pressed, this, &ACPP_PlayerManager::Invisibility );
	PlayerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACPP_PlayerManager::StartShooting );
	PlayerInputComponent->BindAction( "Shoot", IE_Released, this, &ACPP_PlayerManager::StopShooting );
	PlayerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACPP_PlayerManager::StartAim );
	PlayerInputComponent->BindAction( "Aim", IE_Released, this, &ACPP_PlayerManager::StopAim );
	PlayerInputComponent->BindAction( "CoverButton", IE_Pressed, this, &ACPP_PlayerManager::WallTrace );
	PlayerInputComponent->BindAction( "DistractEnemy", IE_Pressed, this, &ACPP_PlayerManager::DistractEnemy );

}

void ACPP_PlayerManager::Turn( float inputAxis )
{
	AddControllerYawInput( inputAxis );

	float X = GetVelocity().X;
	float Y = GetVelocity().Y;
	float Z = GetVelocity().Z;

	float vectorLength = FMath::Sqrt( X * X + Y * Y + Z * Z );

	if( vectorLength == 0 )
	{
		// Right
		if( inputAxis > 0.3f )
		{
			m_turnRight = true;
		}
		else
		{
			m_turnRight = false;
		}
		// Left
		if( inputAxis < -0.3f )
		{
			m_turnLeft = true;
		}
		else
		{
			m_turnLeft = false;

		}

	}
	else
	{
		m_turnRight = false;
		m_turnLeft = false;
	}

}



bool ACPP_PlayerManager::GetTurnRight() const
{
	return m_turnRight;
}

bool ACPP_PlayerManager::GetTurnLeft() const
{
	return m_turnLeft;
}

void ACPP_PlayerManager::MoveForward( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

		// Add movement in that direction
		AddMovementInput( Direction, inputAxis );

		const FVector location = GetActorLocation();
		//UGameplayStatics::PlaySoundAtLocation( GetWorld(), footstepsSFX, GetActorLocation() );

		// Report noise to event so that enemy can 'hear' it
		//UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 0.1f, this, 0.0f, noiseTag );
	}

}

void ACPP_PlayerManager::MoveRight( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) && ( m_isInCover == false ) )
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		// Add movement in that direction
		AddMovementInput( Direction, inputAxis );
	}
	else if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) && ( m_isInCover == true ) )
	{
		CoverTrace( inputAxis );
	}
}

void ACPP_PlayerManager::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ACPP_PlayerManager::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

}

void ACPP_PlayerManager::BeginCrouch()
{
	//// Set bool to opposite value
	//m_isCrouched = !m_isCrouched;

	//// If crouched is true, crouch
	//if( m_isCrouched == true )
	//{
	//	Crouch();

	//	// Move camera
	//	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 140.0f ) );
	//}

	//// Else, stop crouching
	//else
	//{
	//	UnCrouch();

	//	// Move camera
	//	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	//}
}

void ACPP_PlayerManager::EndCrouch()
{
	//UnCrouch();
}

void ACPP_PlayerManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	AmmoTick();

	InvisibilityTick( DeltaTime );

	EquipWeaponTick();

	HitmarkerTick();

	if( health == 0 )
	{
		PlayerDead();
	}
	
	TakedownTrace();
	//userInterfaceDelegate();
}

void ACPP_PlayerManager::InvisibilityTick( float DeltaTime )
{
	InvisibilityEvaluation( DeltaTime );

	InvisibilityMaterial();
}

void ACPP_PlayerManager::InvisibilityEvaluation( float DeltaTime )
{
	if( m_invisibilityPercent > 0.0f && m_invisibility )
	{
		GetWorld()->GetTimerManager().SetTimer( m_invisiblityTimer, this, &ACPP_PlayerManager::InvisibilityFinished, 1.0f, true );
		m_invisibilityPercent -= DeltaTime * m_invisibilityTimeDrain;
		m_aimingIn = false;
	}

	if( m_invisibilityPercent < m_invisibilityFull && !m_invisibility || m_invisibilityPercent == 0 )
	{
		m_invisibilityPercent += ( DeltaTime * m_invisibilityTimeMultiplier );
	}

	/*if( m_invisibilityPercent == 0 )
	{
		m_invisibilityPercent += ( DeltaTime * m_invisibilityTimeMultiplier );
	}*/
}

void ACPP_PlayerManager::InvisibilityMaterial()
{
	// If player is invisible
	if( m_invisibility )
	{
		for( int i = 0; i < GetMesh()->GetNumMaterials(); i++ )
		{
			GetMesh()->SetMaterial( i, invisibleMaterial );
		}
	}
	else
	{
		// Back to original texture
		//m_dynamicMaterial->SetScalarParameterValue( TEXT( "EmissiveStrength" ), 0 );
		
		for( int i = 0; i < GetMesh()->GetNumMaterials(); i++ )
		{
			GetMesh()->SetMaterial( i, m_material[ i ] );
		}

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
	
	AmmoEvaluation( m_ammoAR, m_reserveAR, m_fullMagAR );

	AmmoEvaluation( m_ammoPistol, m_reservePistol, m_fullMagPistol );

	UserInterfaceDelegate();
	
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

bool ACPP_PlayerManager::GetCanTakedown() const
{
	return m_canTakedown;
}

void ACPP_PlayerManager::Takedown()
{
	if( m_canTakedown )
	{
		
		//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, managerAI->GetName() );
    
		FActorSpawnParameters spawnInfo;
		
		AAIController* controllerAI = GetWorld()->SpawnActor<AAIController>( GetActorLocation(), GetActorRotation(), spawnInfo );
		
		if( controllerAI ) 
		{
			ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( TakedownTrace() );
			if( managerAI )
			{
				// Disable AI movement
				managerAI->GetCharacterMovement()->DisableMovement();

				// Disable Player movement
				GetCharacterMovement()->DisableMovement();

				// Stop controller from possessing the player
				GetController()->UnPossess();

				// Find the location behind the AI for Player to move to
				FVector moveToLocation = GetActorLocation() + ( GetActorForwardVector() * -50.0f );

				// Hold the player controller
				m_pPlayerController = GetWorld()->GetFirstPlayerController();

				// Have AI controller control Player 
				controllerAI->Possess( this );

				// Have Player automatically move towards location of the AI
				controllerAI->MoveToLocation( moveToLocation );
				controllerAI->MoveToActor( managerAI, 50.0f );
				
				if( animTakedown )
				{
					// Play animation
					PlayAnimMontage( animTakedown );

					// AI behaviour for when they are being takedown
					managerAI->Takedown();

					// Timer for actions when takedown is completed
					FTimerHandle delayTimer;
					GetWorld()->GetTimerManager().SetTimer( delayTimer, this, &ACPP_PlayerManager::AnimationExecuted, m_animCompletion, false );
				}
			}
		}
		

		

	}
}

AActor* ACPP_PlayerManager::TakedownTrace()
{
	FVector start = GetActorLocation() + 100.0f;
	FVector end = ( GetActorForwardVector() * takedownTraceDistance ) + ( start - 100.0f );
	FHitResult hit;
	FCollisionQueryParams traceParams; // Trace parameters

	//DrawDebugLine( GetWorld(), start, end, FColor::Red );
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::ECC_Camera, traceParams );
	if( bHit )
	{
		float dotProduct = FVector::DotProduct( GetActorForwardVector(), hit.GetActor()->GetActorForwardVector() );

		bool nearlyEqual = UKismetMathLibrary::NearlyEqual_FloatFloat( dotProduct, 1.0f, 0.1f );
		if( nearlyEqual )
		{
			// Get AI Manager
			ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( hit.Actor );
			if( managerAI )
			{
				if( managerAI->GetCanTakedown() == true )
				{
					m_canTakedown = true;
					takedownAvailable = true;
					//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, hit.GetActor()->GetName() );
					return hit.GetActor();

				}

			}
		}
	}
	takedownAvailable = false;
	m_canTakedown = false;
	return NULL;


}




void ACPP_PlayerManager::Invisibility()
{
	if( m_invisibility )
	{
		// If invisiblity bar is above 0
		if( m_invisibilityPercent > 0.0f )
		{
			m_invisibility = !m_invisibility;
		}
	}

	if( !m_invisibility )
	{
		if( m_invisibilityPercent >= 100.0f )
		{
			m_invisibility = !m_invisibility;
		}
	}
	
}

bool ACPP_PlayerManager::GetInvisibilityStatus() const
{
	return m_invisibility;
}

void ACPP_PlayerManager::InvisibilityFinished()
{
	m_invisibility = false;
}

void ACPP_PlayerManager::Respawn()
{
	// Play death anim montage
	PlayAnimMontage( deadAnim );

	// Reset health
	//health = defaultHealth;
}

void ACPP_PlayerManager::IncreaseAmmoCount( int ammo )
{
	// Increase ammunition
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

		// Attach to selected component
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

		// Attach to selected component
		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolSocket );
		bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolMuzzleSocket );

	}
	if( m_currentlyEquipped == 2 )
	{
		// Set bools
		m_assaultRifle = false;
		m_pistol = false;
		m_throwable = true;

		// Attach to selected component
		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket );

	}
}

void ACPP_PlayerManager::ChangeWeapons( float inputAxis )
{
	// Value of which weapon is chosen from mouse wheel
	int value = UKismetMathLibrary::FTrunc( inputAxis );
	int changeWeaponValue = value + m_currentlyEquipped;

	if( m_weaponInventory.IsValidIndex( changeWeaponValue ) )
	{
		// Equip weapon of that value
		m_currentlyEquipped = changeWeaponValue;
	}
	else 
	{
		if( changeWeaponValue < 0 )
		{
			// Weapon value is equal to the final weapon
			m_currentlyEquipped = m_weaponInventory.Num() - 1;
		}
		else
		{
			// Weapon value is equal to the first weapon
			m_currentlyEquipped = 0;
		}
	}

	// Equip that weapon
	EquipGun( m_weaponInventory );
}

bool ACPP_PlayerManager::GetHitmarkerActive() const
{
	return m_hitmarkerActive;
}

bool ACPP_PlayerManager::GetAssaultRifle() const
{
	return m_assaultRifle;
}

bool ACPP_PlayerManager::GetPistol() const
{
	return m_pistol;
}

bool ACPP_PlayerManager::GetThrowable() const
{
	return m_throwable;
}

void ACPP_PlayerManager::SetHitmarkerActive( bool hitmarkerActive )
{
	if( m_ammoAR > 0 || m_ammoPistol > 0 )
	{
		m_hitmarkerActive = hitmarkerActive;
	}
}

void ACPP_PlayerManager::DistractEnemy()
{
	if( callEnemy )
	{
		// Play sound at player location
		const FVector location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), callEnemy, location );

		// Report noise to event so that enemy can 'hear' it
		UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 1.0f, this, 0.0f, noiseTag );
	}
}

void ACPP_PlayerManager::SetDeathHitmarkerActive( bool deathHitmarkerActive )
{
	m_deathHitmarkerActive = deathHitmarkerActive;
}

bool ACPP_PlayerManager::GetDeathHitmarkerActive() const
{
	return m_deathHitmarkerActive;
}


void ACPP_PlayerManager::AnimationExecuted()
{
	if( m_pPlayerController )
	{
		m_pPlayerController->Possess( this );
	}

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
}

void ACPP_PlayerManager::UserInterfaceDelegate()
{
	// Broadcast delegates depending on which ammunition counter to display
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

void ACPP_PlayerManager::StartAim()
{
	if( !m_invisibility )
	{
		// Set bool to true
		m_aimingIn = true;

		// Zoom in camera
		springArmComp->TargetArmLength = 100.0f;
	}
}

void ACPP_PlayerManager::EquipWeaponTick()
{
	// If player is aiming in, show weapon
	m_aimingIn ? m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( true ) : m_weaponInventory[ m_currentlyEquipped ]->SetVisibility( false );
}

void ACPP_PlayerManager::HitmarkerTick()
{
	if( m_hitmarkerActive )
	{
		// Set timer to deactive hitmarker bool
		GetWorld()->GetTimerManager().SetTimer( m_hitmarkerTimer, this, &ACPP_PlayerManager::HitmarkerFinished, 1.0f, true );
	}
}

float ACPP_PlayerManager::GetHealth() const
{
	return health;
}

void ACPP_PlayerManager::PlayerDead()
{
	// Disable character movement
	GetCharacterMovement()->DisableMovement();

	// Disable Player input
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if( playerController )
	{
		DisableInput( playerController );
	}
}

void ACPP_PlayerManager::Reset()
{
}

void ACPP_PlayerManager::StopAim()
{
	m_aimingIn = false;
	springArmComp->TargetArmLength = 200.0f;
	if( m_throwable )
	{
		//DestroyPredictionSpline();
	}
}

void ACPP_PlayerManager::StartCover( FHitResult hit )
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( true );
	GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );
	bUseControllerRotationYaw = false;
	m_isInCover = true;

}

void ACPP_PlayerManager::WallTrace()
{
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace
	const FVector start = GetActorLocation();
	const FVector end = ( GetActorForwardVector() * 100.0f ) + GetActorLocation();

	// Draw a line for debug
	//DrawDebugLine( GetWorld(), start, end, FColor::Yellow, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTracer ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );

	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

	if( bHit )
	{
		StartCover( hit );
	}
	else
	{
		StopCover();
	}
}

void ACPP_PlayerManager::StopCover()
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( false );
	bUseControllerRotationYaw = true;
	m_isInCover = false;
}

bool ACPP_PlayerManager::CoverTrace( float inputAxis )
{
	bool rightHit = RightCoverTrace();
	bool leftHit = LeftCoverTrace();

	if( rightHit && leftHit )
	{
		// Start and end of line trace
		const FVector start = GetActorLocation();
		const FVector end = GetActorLocation() + ( GetCharacterMovement()->GetPlaneConstraintNormal() * 200.0f );

		// Draw a line for debug
		DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

		FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

		// Hit result
		FHitResult hit( ForceInit );
		bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

		if( bHit )
		{
			GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );

			// Rotation
			const FRotator rotation = Controller->GetControlRotation();
			const FRotator yawRotation( 0.0f, rotation.Yaw, 0.0f );

			// Get forward vector
			const FVector direction = FRotationMatrix( yawRotation ).GetUnitAxis( EAxis::Y );

			// Add movement in that direction
			AddMovementInput( direction, inputAxis );

		}
		return bHit;
	}

	else
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		if( inputAxis == 1.0f && rightHit )
		{
			// Add movement in that direction
			AddMovementInput( Direction, inputAxis );
		}
		else if( inputAxis == -1.0f && leftHit )
		{
			// Add movement in that direction
			AddMovementInput( Direction, inputAxis );
		}

		return false;
	}
	//return false;
}

void ACPP_PlayerManager::StartShooting()
{
	if( m_aimingIn )
	{
		if( m_assaultRifle )
		{
			if( m_ammoAR > 0 )
			{
				m_isShooting = true;

				//m_ammoAR -= 1;

				ShootProjectile();

				GetWorld()->GetTimerManager().SetTimer( m_shootTime, this, &ACPP_CharacterManager::ShootProjectile, timeBetweenShots, true );
			}

			if( m_ammoAR <= 0 )
			{
				m_isShooting = false;

				GetWorld()->GetTimerManager().SetTimer( m_reloadTime, this, &ACPP_CharacterManager::Reloaded, m_reloadAnimTime, true );
			}

		}

		if( m_pistol )
		{
			if( m_ammoPistol > 0 )
			{
				m_isShooting = true;

				//m_ammoPistol -= 1;

				ShootProjectile();
			}

		}

		if( m_throwable )
		{
			if( m_throwableAmount == 1 )
			{
				//ThrowObject();
			}
		}

	}
}



void ACPP_PlayerManager::ThrowObject()
{

	ACPP_Throwable* throwableRef = Cast<ACPP_Throwable>( throwable->GetChildActor() );
	if( throwableRef )
	{
		throwableRef->ThrowObject( UKismetMathLibrary::GetForwardVector( GetControlRotation() ) );
		m_throwableAmount--;

		if( animThrow )
		{
			PlayAnimMontage( animThrow );
		}
	}
	else
	{
	}

}

void ACPP_PlayerManager::CreatePredictionSpline()
{
	
}

void ACPP_PlayerManager::DestroyPredictionSpline()
{

}

void ACPP_PlayerManager::DestroyPredictionMeshes()
{
	
}

void ACPP_PlayerManager::DrawPredictionSpline()
{
	
}

bool ACPP_PlayerManager::GetIsCrouched()
{
	return m_isCrouched;
}

bool ACPP_PlayerManager::RightCoverTrace()
{
	// Start and end of line trace
	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );
	const FVector start = GetActorLocation() + ( movementDirection * 45.0f );
	const FVector end = ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f ) * 200.0f ) + start;

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool rightHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return rightHit;
}

bool ACPP_PlayerManager::LeftCoverTrace()
{
	// Left 
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace

	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );
	const FVector start = GetActorLocation() + ( movementDirection * 45.0f );
	const FVector end = ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f ) * 200.0f ) + start;

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool leftHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return leftHit;
}