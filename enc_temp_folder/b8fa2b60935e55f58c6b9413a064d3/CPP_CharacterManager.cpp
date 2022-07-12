// Fill out your copyright notice in the Description page of Project Settings.
#define COLLISION_COVER ECC_GameTraceChannel2

#include "CPP_CharacterManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACPP_CharacterManager::ACPP_CharacterManager()
	:m_isCrouched( true )
	,bulletComp()
	,m_projectileRange( 1000.0f )
	,m_muzzleRotationPitch( 3.0f )
	,m_canBeShot( true )
	,m_weaponRange( 20000.0f )
	,m_aimingIn( false )
	,timeBetweenShots( 0.15f )
	,m_shootTime()
	,m_isInCover()
	,weaponSocket( TEXT( "GunSocket" ) )
	,muzzleSocket( TEXT( "MuzzleSocket" ) )
	,headSocket( TEXT( "Head" ) )
	,m_ammoCount( 30 )
	,m_reloadTime()
	,m_reloadAnimTime( 3.0f )
	,m_reserveAmmo( 0 )
	,m_fullMag( 30 )
	,m_assaultRifle ( true )
	,m_pistol( false )
	,recoil( -0.1f )
	,m_shotInHead( false )
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialise components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	bulletComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BulletComp" ) );
	headCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "HeadCollision" ) );

	// Set relative location and rotation of the skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation( FVector( 0.0f, 0.0f, -90.0f ), FQuat( FRotator( 0.0f, -90.0f, 0.0f ) ) );

	// Attatch components to mesh
	springArmComp->SetupAttachment( GetMesh() );
	cameraComp->SetupAttachment( springArmComp, USpringArmComponent::SocketName );

	// Set class variables of the spring arm
	springArmComp->bUsePawnControlRotation = true;
	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	springArmComp->TargetArmLength = 200.0f;


	//gunComp->AttachTo( GetMesh(), weaponSocket, EAttachLocation::SnapToTargetIncludingScale, false );
	gunComp->SetupAttachment( GetMesh(), weaponSocket );
	bulletComp->SetupAttachment( gunComp );
	//bulletComp->AttachTo( gunComp, weaponSocket, EAttachLocation::SnapToTarget, true );
	//bulletComp->SetRelativeLocation( ( gunComp->GetRelativeLocation() ) );


	headCollision->SetupAttachment( GetMesh(), headSocket );

	// Set class variables of Character Movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	// Set nav agent property for crouching to true
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

}

// Called when the game starts or when spawned
void ACPP_CharacterManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_CharacterManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	// Handle reserve ammunition
	if( m_ammoCount > m_fullMag )
	{
		m_reserveAmmo = m_ammoCount - m_fullMag;
		m_ammoCount = m_fullMag;
	}

	// When reloading
	if( m_ammoCount == 0 )
	{
		m_isShooting = false;
		// If theres not enough  reserve for a full mag
		if( m_reserveAmmo > 0 && m_reserveAmmo < m_fullMag )
		{
			m_ammoCount = m_reserveAmmo;
			m_reserveAmmo = m_fullMag;
		}

		// If there is enough for a full mag
		if( m_reserveAmmo >= m_fullMag )
		{
			m_ammoCount = m_fullMag;
			m_reserveAmmo -= m_fullMag;
		}
	}

	

	if( m_isShooting )
	{
		// Recoil
		AddControllerPitchInput( recoil );

		
	}
}

// Called to bind functionality to input
void ACPP_CharacterManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( "MoveForward", this, &ACPP_CharacterManager::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACPP_CharacterManager::MoveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );
	PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACPP_CharacterManager::BeginCrouch );
	//PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &ACPP_CharacterManager::EndCrouch );
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACPP_CharacterManager::BeginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &ACPP_CharacterManager::EndSprint );



}

void ACPP_CharacterManager::MoveForward( float inputAxis )
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
	}

}

void ACPP_CharacterManager::MoveRight( float inputAxis )
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

void ACPP_CharacterManager::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ACPP_CharacterManager::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

}

void ACPP_CharacterManager::BeginCrouch()
{
	// Set bool to opposite value
	m_isCrouched = !m_isCrouched;

	// If crouched is true, crouch
	if( m_isCrouched == true )
	{
		Crouch();

		// Move camera
		springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 140.0f ) );
	}

	// Else, stop crouching
	else
	{
		UnCrouch();

		// Move camera
		springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	}
}

void ACPP_CharacterManager::EndCrouch()
{
	UnCrouch();
}


void ACPP_CharacterManager::UpdateWalkSpeed( float speed )
{
	// Set walk speed
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

FHitResult ACPP_CharacterManager::RaycastShot()
{
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace
	const FVector start = cameraComp->GetComponentLocation();
	const FVector end = ( cameraComp->GetForwardVector() * m_weaponRange ) + start;

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Blue, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( Shoot ), true, GetInstigator() );
	
	// Hit result
	FHitResult hit( ForceInit );

	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_WorldDynamic, traceParams );

	// Muzzle offset
	m_muzzleOffset.Set( 200.0f, 0.0f, 0.0f );

	// Transform Muzzleoffset from camera space to world space
	FVector muzzleLocation = cameraLocation + FTransform( cameraRotation ).TransformVector( m_muzzleOffset );

	// Skew aim to be upwards
	FRotator muzzleRotation = cameraRotation;
	muzzleRotation.Pitch += m_muzzleRotationPitch;

	if( bHit )
	{
		// Box where collision has occured
		DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );
	}
	
	return hit;
}

void ACPP_CharacterManager::StartShooting()
{
	if( m_aimingIn )
	{
		
		if( m_ammoCount > 0 )
		{
			m_isShooting = true;

			// Call shoot function
			ShootProjectile();

			// Set timer for automatic shooting
			GetWorld()->GetTimerManager().SetTimer( m_shootTime, this, &ACPP_CharacterManager::ShootProjectile, timeBetweenShots, true );

			// Recoil
			//AddControllerPitchInput( recoil );
		}

		if( m_ammoCount <= 0 )
		{
			GetWorld()->GetTimerManager().SetTimer( m_reloadTime, this, &ACPP_CharacterManager::Reloaded, m_reloadAnimTime, true );
		}

	}
}

void ACPP_CharacterManager::StopShooting()
{
	m_isShooting = false;
	GetWorld()->GetTimerManager().ClearTimer( m_shootTime );
}

void ACPP_CharacterManager::StartAim()
{
	m_aimingIn = true;
	springArmComp->TargetArmLength = 100.0f;
}

void ACPP_CharacterManager::StopAim()
{
	m_aimingIn = false;
	springArmComp->TargetArmLength = 200.0f;
}

void ACPP_CharacterManager::ShootProjectile()
{
	if( m_ammoCount > 0 )
	{
		// Spawn muzzle flash
		UGameplayStatics::SpawnEmitterAttached( animShoot, bulletComp, "MyAttachPoint", bulletComp->GetRelativeLocation(), FRotator( 0.0f, 90.0f, 0.0f ), FVector( 0.1f, 0.1f, 0.1f ) );

		// Get the hit that has been returned
		FHitResult hit = RaycastShot();

		// Decrement ammo count
		m_ammoCount -= 1.0f;
		FString ammoDebug = FString::SanitizeFloat( m_ammoCount );

		// Debug
		//GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, ammoDebug );

		// Get the actor that has been hit
		ACPP_CharacterManager* hitActor = Cast<ACPP_CharacterManager>( hit.Actor );

		// If the actor can be shot and has been hit
		if( hitActor && m_canBeShot )
		{
		
			if( hit.GetComponent()->ComponentHasTag( headSocket ) )
			{
				{
					hitActor->HasBeenShotInTheHead( true );
				}
			}

			// Call function that decides what happens when hit 
			hitActor->TakeAttack(); // Function is overridable 
		}
	
	}
}

void ACPP_CharacterManager::TakeAttack()
{
	// -- IMPLEMENTATION REQUIRED --
}

bool ACPP_CharacterManager::GetIsCrouched()
{
	return m_isCrouched;
}

void ACPP_CharacterManager::Reloaded()
{
	GetWorld()->GetTimerManager().ClearTimer( m_reloadTime );
	m_ammoCount = 30.0f;
}

void ACPP_CharacterManager::StartCover( FHitResult hit )
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( true );
	GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );
	bUseControllerRotationYaw = false;
	m_isInCover = true;

}

void ACPP_CharacterManager::StopCover()
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( false );
	bUseControllerRotationYaw = true;
	m_isInCover = false;
}

void ACPP_CharacterManager::WallTrace()
{
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace
	const FVector start = GetActorLocation();
	const FVector end = ( GetActorForwardVector() * 100.0f ) + GetActorLocation();

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Yellow, false, 5.0f );

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

void ACPP_CharacterManager::TakeCover()
{
}

bool ACPP_CharacterManager::CoverTrace( float inputAxis )
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
			const FRotator yawRotation ( 0.0f, rotation.Yaw, 0.0f );

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

bool ACPP_CharacterManager::RightCoverTrace()
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

bool ACPP_CharacterManager::LeftCoverTrace()
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

void ACPP_CharacterManager::EnemyShoot()
{
	ShootProjectile();
}

bool ACPP_CharacterManager::GetIsAimedIn()
{
	return m_aimingIn;
}

void ACPP_CharacterManager::HasBeenShotInTheHead( bool boolean )
{
	m_shotInHead = boolean;
}
