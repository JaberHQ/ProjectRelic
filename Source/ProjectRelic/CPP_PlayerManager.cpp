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
	,m_chanceOfHit( 0.5f )
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
	,m_dead( false )
	,m_acceptableRadiusToEnemy( 50.0f )
	,m_takedownTraceValue( 100.0f )
	,m_aimingInValue( 100.0f )
	,m_aimingInReturnValue( 200.0f )
	,m_rightAxisValue ( 0.3f )
	,m_invisiblityTimerFinished( 1.0f )
	,m_increaseARAmmo( 10 )
	,m_increasePistolAmmo( 5 )
	,m_percentageMutliplier( 100.0f )
	,m_hitmarkerAnimComplete( 1.0f )
	,m_wallTraceMultipler( 100.0f )
	,m_assaultRifleValue( 0 )
	,m_pistolValue( 1 )
	,m_throwableValue( 2 )
	,m_coverTraceValue( 200.0f )
	,m_throwableFull( 1 )
	,animThrow()
	,m_rightCoverTraceMultiplier( 45.0f )
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
	// Null pointer
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

	// Loop through player materials
	for( int i = 0; i < GetMesh()->GetNumMaterials(); i++ )
	{
		// Add it to the array
		m_material.Add( GetMesh()->GetMaterial( i ) );
	}

}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );
	PrimaryActorTick.bCanEverTick = true;

	// Bind Axis
	PlayerInputComponent->BindAxis( "MoveForward", this, &ACPP_PlayerManager::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACPP_PlayerManager::MoveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &ACPP_PlayerManager::Turn );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );
	PlayerInputComponent->BindAxis( "ChangeWeapons", this, &ACPP_PlayerManager::ChangeWeapons );

	// Bind Action
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACPP_PlayerManager::BeginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &ACPP_PlayerManager::EndSprint );
	PlayerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &ACPP_PlayerManager::Takedown );
	PlayerInputComponent->BindAction( "PowerUp", IE_Pressed, this, &ACPP_PlayerManager::Invisibility );
	PlayerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACPP_PlayerManager::StartShooting );
	PlayerInputComponent->BindAction( "Shoot", IE_Released, this, &ACPP_PlayerManager::StopShooting );
	PlayerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACPP_PlayerManager::StartAim );
	PlayerInputComponent->BindAction( "Aim", IE_Released, this, &ACPP_PlayerManager::StopAim );
	PlayerInputComponent->BindAction( "CoverButton", IE_Pressed, this, &ACPP_PlayerManager::WallTrace );
	PlayerInputComponent->BindAction( "DistractEnemy", IE_Pressed, this, &ACPP_PlayerManager::DistractEnemy );
	PlayerInputComponent->BindAction( "XrayVision", IE_Pressed, this, &ACPP_PlayerManager::Xray );

	// Depreciated actions
	//PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	//PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );
	//PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACPP_CharacterManager::BeginCrouch );
	//PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &ACPP_CharacterManager::EndCrouch );
}

void ACPP_PlayerManager::Turn( float inputAxis )
{
	AddControllerYawInput( inputAxis );

	// Get turn velocity
	float X = GetVelocity().X;
	float Y = GetVelocity().Y;
	float Z = GetVelocity().Z;

	float vectorLength = FMath::Sqrt( X * X + Y * Y + Z * Z );

	if( vectorLength == 0 )
	{
		// Right
		inputAxis > m_rightAxisValue ? m_turnRight = true : m_turnRight = false;

		// Left
		inputAxis < -m_rightAxisValue ? m_turnLeft = true : m_turnLeft = false;
		
	}
	else
	{
		// Set bool to false
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

		// Set xray off
		m_xray = false;
	}

}

void ACPP_PlayerManager::MoveRight( float inputAxis )
{
	if( m_isInCover )
	{
		if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
		CoverTrace( inputAxis );
	}
	else
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
			m_xray = false;
		}
	}
	
}

void ACPP_PlayerManager::BeginSprint()
{
	//GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ACPP_PlayerManager::EndSprint()
{
	//GetCharacterMovement()->MaxWalkSpeed = 600.0f;

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

	TakedownTrace();

	if( health == 0 )
		PlayerDead();
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
		// Set invsiiblity timer
		GetWorld()->GetTimerManager().SetTimer( m_invisiblityTimer, this, &ACPP_PlayerManager::InvisibilityFinished, m_invisiblityTimerFinished, true );

		// Decrement the invisibility bar 
		m_invisibilityPercent -= DeltaTime * m_invisibilityTimeDrain;

		// Stop player from being able to aim
		m_aimingIn = false;
	}

	// If invisibility is off and less than full
	if( ( m_invisibilityPercent < m_invisibilityFull && !m_invisibility ) || m_invisibilityPercent == 0 )
		m_invisibilityPercent += ( DeltaTime * m_invisibilityTimeMultiplier ); // Increase invisiblity again
}

void ACPP_PlayerManager::InvisibilityMaterial()
{
	// If player is invisible
	if( m_invisibility )
	{
		// Change player material to invisiblity to give invisibility look
		for( int i = 0; i < GetMesh()->GetNumMaterials(); i++ )
		{
			GetMesh()->SetMaterial( i, invisibleMaterial );
		}
	}
	else
	{
		// Change material to player material
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

		// Actor spawn parameters
		FActorSpawnParameters spawnInfo;
		
		
		AAIController* controllerAI = GetWorld()->SpawnActor<AAIController>( GetActorLocation(), GetActorRotation(), spawnInfo ); // Controller reference
		
		if( controllerAI ) 
		{
			ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( TakedownTrace() ); // AI reference
			if( managerAI )
			{
				// Disable AI movement
				managerAI->GetCharacterMovement()->DisableMovement();

				// Disable Player movement
				GetCharacterMovement()->DisableMovement();

				// Stop controller from possessing the player
				GetController()->UnPossess();

				// Find the location behind the AI for Player to move to
				FVector moveToLocation = GetActorLocation() + ( GetActorForwardVector() * -m_acceptableRadiusToEnemy );

				// Hold the player controller
				m_pPlayerController = GetWorld()->GetFirstPlayerController();

				// Have AI controller control Player 
				controllerAI->Possess( this );

				// Have Player automatically move towards location of the AI
				controllerAI->MoveToLocation( moveToLocation );
				controllerAI->MoveToActor( managerAI, m_acceptableRadiusToEnemy );
				
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
	// Parameters of line trace
	FVector start = GetActorLocation() + m_takedownTraceValue;
	FVector end = ( GetActorForwardVector() * takedownTraceDistance ) + ( start - m_takedownTraceValue );
	FHitResult hit;
	FCollisionQueryParams traceParams; // Trace parameters

	// Line trace 
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::ECC_Camera, traceParams );
	
	// If hit
	if( bHit )
	{

		// Work out the dot product of player and the hit actor which should be the enemy
		float dotProduct = FVector::DotProduct( GetActorForwardVector(), hit.GetActor()->GetActorForwardVector() );

		// If dot product is nearly equal to 1
		bool nearlyEqual = UKismetMathLibrary::NearlyEqual_FloatFloat( dotProduct, 1.0f, 0.1f );
		if( nearlyEqual )
		{
			// Get AI Manager
			ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( hit.Actor );
			if( managerAI )
			{
				// If AI can be taken down
				if( managerAI->GetCanTakedown() == true )
				{

					// Set bools to true
					m_canTakedown = true;
					takedownAvailable = true;

					// Return the actor that was hit by the line trace
					return hit.GetActor();

				}

			}
		}
	}

	// Set bools to false and return null
	takedownAvailable = false;
	m_canTakedown = false;
	return NULL;


}




void ACPP_PlayerManager::Invisibility()
{
	// Invisibility on
	if( m_invisibility )
	{
		// If invisiblity bar is above 0
		if( m_invisibilityPercent > 0.0f )
		{
			// Change invisibility bool
			m_invisibility = !m_invisibility;
		}
	}

	// Invisibility off
	if( !m_invisibility )
	{
		// If inivsibility is off and the bar is full
		if( m_invisibilityPercent >= m_invisibilityFull )
		{
			// Change invisibility bool
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
}

void ACPP_PlayerManager::Xray()
{
	// Change xray bool
	m_xray = !m_xray;
}

void ACPP_PlayerManager::IncreaseAmmoCount( int ammo )
{
	// Increase ammunition
	m_ammoAR += m_increaseARAmmo;
	m_ammoPistol += m_increasePistolAmmo;	
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
	if( m_currentlyEquipped == m_assaultRifleValue )
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
	if( m_currentlyEquipped == m_pistolValue )
	{
		// Set bools
		m_assaultRifle = false;
		m_pistol = true;
		m_throwable = false;

		// Attach to selected component
		WeaponInventory[ m_currentlyEquipped ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolSocket );
		bulletComp->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_pistolMuzzleSocket );

	}
	if( m_currentlyEquipped == m_throwableValue )
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
		// Posses the player again
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
		health > 0.0f ? health -= m_shotDamage : m_dead = true;

		if( m_dead )
			PlayAnimMontage( deadAnim );
	}
}

void ACPP_PlayerManager::UserInterfaceDelegate()
{
	// Broadcast delegates depending on which ammunition counter to display
	if( m_assaultRifle )
	{
		userInterfaceD.Broadcast( health / m_percentageMutliplier, m_invisibilityPercent / m_percentageMutliplier, m_ammoAR, m_reserveAR, m_assaultRifle, m_pistol, m_throwable );
	}
	if( m_pistol )
	{
		userInterfaceD.Broadcast( health / m_percentageMutliplier, m_invisibilityPercent / m_percentageMutliplier, m_ammoPistol, m_reservePistol, m_assaultRifle, m_pistol, m_throwable );
	}
	if( m_throwable )
	{
		userInterfaceD.Broadcast( health / m_percentageMutliplier, m_invisibilityPercent / m_percentageMutliplier, m_throwableAmount, 0, m_assaultRifle, m_pistol, m_throwable );
	}
}

bool ACPP_PlayerManager::GetDead() const
{
	return m_dead;
}

void ACPP_PlayerManager::SetThrowable( bool boolean )
{
	m_throwable = boolean;
}

bool ACPP_PlayerManager::GetXray() const
{
	return m_xray;
}

void ACPP_PlayerManager::StartAim()
{
	if( !m_invisibility )
	{
		// Set bool to true
		m_aimingIn = true;

		// Zoom in camera
		springArmComp->TargetArmLength = m_aimingInValue;
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
		GetWorld()->GetTimerManager().SetTimer( m_hitmarkerTimer, this, &ACPP_PlayerManager::HitmarkerFinished, m_hitmarkerAnimComplete, true );
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
		DisableInput( playerController );
}

void ACPP_PlayerManager::Reset()
{
}

void ACPP_PlayerManager::StopAim()
{
	// Stop aiming in bool
	m_aimingIn = false;

	// Return spring arm length
	springArmComp->TargetArmLength = m_aimingInReturnValue;
}

void ACPP_PlayerManager::WallTrace()
{
	if( m_isInCover )
	{
		StopCover();
	}
	else
	{
		// Start and end of line trace
		const FVector start = GetActorLocation();
		const FVector end = ( GetActorForwardVector() * m_wallTraceMultipler ) + GetActorLocation();

		FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTracer ), true, GetInstigator() );

		// Hit result
		FHitResult hit( ForceInit );

		bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

		// If hit start cover, else stop cover
		bHit ? StartCover( hit ) : StopCover();
	}
	
}

void ACPP_PlayerManager::StartCover( FHitResult hit )
{
	// Set plane constraints
	GetCharacterMovement()->SetPlaneConstraintEnabled( true );
	GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );

	// Turn off rotation yaw
	bUseControllerRotationYaw = false;

	// Set in cover
	m_isInCover = true;

}


void ACPP_PlayerManager::StopCover()
{
	// Stop plane constaint
	GetCharacterMovement()->SetPlaneConstraintEnabled( false );

	// Give back player rotation yaw
	bUseControllerRotationYaw = true;

	// Set cover bool 
	m_isInCover = false;
}

void ACPP_PlayerManager::CoverTrace( float inputAxis )
{
	bool rightHit = RightCoverTrace();
	bool leftHit = LeftCoverTrace();

	const FVector direction = UKismetMathLibrary::GetRightVector( UKismetMathLibrary::MakeRotator( 0.0f, 0.0f, GetControlRotation().Yaw ) );
	if( rightHit && leftHit )
	{
		if( inputAxis != 0.0f )
		{
			const FVector start = GetActorLocation();
			const FVector end = GetActorLocation() + ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f ) * m_coverTraceValue );

			FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

			// Hit result
			FHitResult hit( ForceInit );
			bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

			if( bHit )
			{
				GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );

				// Add movement in that direction
				AddMovementInput( direction, inputAxis );

			}
		}
	}

	if( rightHit || leftHit )
	{
		bool pickA;

		const float signOfInputAxis = UKismetMathLibrary::SignOfFloat( inputAxis );

		signOfInputAxis == 1.0f ? rightHit = true : leftHit = true;

		if( signOfInputAxis && ( rightHit == true || leftHit == true ) )
		 pickA = true;

		const float scale = UKismetMathLibrary::SelectFloat( inputAxis, 0.0f, pickA );

		AddMovementInput( direction, scale );

	}
}

bool ACPP_PlayerManager::RightCoverTrace()
{
	const float m_rightCoverValue = -1.0f;

	// Start and end of line trace
	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() * m_rightCoverValue );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );

	// Start and end
	const FVector start = GetActorLocation() + ( movementDirection * m_rightCoverTraceMultiplier );
	const FVector end = ( GetCharacterMovement()->GetPlaneConstraintNormal() * m_wallTraceMultipler ) + start;

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool rightHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return rightHit;
}

bool ACPP_PlayerManager::LeftCoverTrace()
{

	const float m_leftCoverValue = -1.0f;

	// Start and end of line trace
	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );

	// Start and end
	const FVector start = GetActorLocation() + ( movementDirection * m_rightCoverTraceMultiplier );
	const FVector end = ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * m_leftCoverValue ) * m_wallTraceMultipler ) + start;

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool leftHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return leftHit;
}

void ACPP_PlayerManager::StartShooting()
{
	if( m_aimingIn )
	{
		if( m_assaultRifle )
		{
			if( m_ammoAR > 0 )
			{
				// Shooting is true
				m_isShooting = true;

				// Get actor location
				const FVector location = GetActorLocation();
				
				// Play shooting sound that AI will hear
				UGameplayStatics::PlaySoundAtLocation( GetWorld(), shootSFX, gunComp->GetRelativeLocation(), 0.1f );
				UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 1.0f, this, 0.0f, noiseTag );
				
				// Shoot Raycast
				ShootProjectile();

				// Set shoot timer
				GetWorld()->GetTimerManager().SetTimer( m_shootTime, this, &ACPP_CharacterManager::ShootProjectile, timeBetweenShots, true );
			}

			if( m_ammoAR <= 0 )
			{
				// Stop shooting
				m_isShooting = false;

				// Get player location
				const FVector location = GetActorLocation();

				// Play no ammo sound
				UGameplayStatics::PlaySoundAtLocation( GetWorld(), pistolShootSFX, bulletComp->GetRelativeLocation(), 0.1f );
				UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 1.0f, this, 0.0f, noiseTag );

				// Set reload timer
				GetWorld()->GetTimerManager().SetTimer( m_reloadTime, this, &ACPP_CharacterManager::Reloaded, m_reloadAnimTime, true );
			}

		}

		if( m_pistol )
		{
			// If there is pistol ammo
			if( m_ammoPistol > 0 )
			{
				// Shooting is true
				m_isShooting = true;

				// Shoot your projectile
				ShootProjectile();
			}

		}

		if( m_throwable )
		{
			// If there is a throwable object
			if( m_throwableAmount == m_throwableFull )
				// Play animation
				PlayAnimMontage( animThrow );
		}
	}
}

void ACPP_PlayerManager::ThrowObject()
{
	ACPP_Throwable* throwableRef = Cast<ACPP_Throwable>( throwable->GetChildActor() );
	if( throwableRef )
	{
		// Throw object
		throwableRef->ThrowObject( UKismetMathLibrary::GetForwardVector( GetControlRotation() ) );
		
		// Decrement throwable
		m_throwableAmount--;

		if( animThrow )
			// Play anim
			PlayAnimMontage( animThrow );
	}
}

bool ACPP_PlayerManager::GetIsCrouched()
{
	return m_isCrouched;
}

