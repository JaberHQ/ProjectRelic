// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACharacterManager::ACharacterManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating class components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	aDSCameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "ADSCameraComp" ) );

	// Set the location and rotation of the Characrer Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation( FVector( -6.0f, 24.0f, 130.0f ), FQuat( FRotator( 0.0f, 0.0, 90.0f ) ) );

	// Attatch your class Components to the default Skeletal Mesh Component 
	cameraComp->SetupAttachment( GetMesh() );
	cameraComp->AttachTo( GetMesh(), TEXT( "head" ), EAttachLocation::SnapToTargetIncludingScale, true );
	cameraComp->SetRelativeLocation( FVector( 0.0f, 33.0f, 160.0f ) );
	cameraComp->SetRelativeRotation( FRotator( 0.0f, 0.0f, 0.0f ) );
	cameraComp->bUsePawnControlRotation = false;

	gunComp->SetupAttachment( GetMesh() );
	aDSCameraComp->SetupAttachment( gunComp );
	gunComp->AttachTo( GetMesh(), TEXT( "thumb_01_l" ), EAttachLocation::SnapToTargetIncludingScale, true );

	// Set Gun Position

	// Set ADS CameraPosition


	//CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);
	// Setting class variables of the Character movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = false;

	m_holdADS = false;
}

// Called when the game starts or when spawned
void ACharacterManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACharacterManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ACharacterManager::SetupPlayerInputComponent( UInputComponent* playerInputComponent )
{
	Super::SetupPlayerInputComponent( playerInputComponent );

	// Movement
	playerInputComponent->BindAxis( "MoveForward", this, &ACharacterManager::MoveForward );
	playerInputComponent->BindAxis( "MoveRight", this, &ACharacterManager::MoveRight );

	// Mouse rotation
	playerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	playerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	// Jump
	playerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	playerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );

	// Crouch
	playerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACharacterManager::BeginCrouch );
	playerInputComponent->BindAction( "Crouch", IE_Released, this, &ACharacterManager::EndCrouch );

	// Sprint
	playerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACharacterManager::BeginSprint );
	playerInputComponent->BindAction( "Sprint", IE_Released, this, &ACharacterManager::EndSprint );

	// Aim
	playerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACharacterManager::AimIn );
	playerInputComponent->BindAction( "Aim", IE_Released, this, &ACharacterManager::AimOut );

	// Shoot
	playerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACharacterManager::Shoot );
}


void ACharacterManager::MoveForward( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
	{
		// Find out the forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		AddMovementInput( Direction, inputAxis );
	}
}

void ACharacterManager::MoveRight( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
	{
		// Find out the right direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
		AddMovementInput( Direction, inputAxis );
	}
}

void ACharacterManager::BeginSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ACharacterManager::EndSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ACharacterManager::BeginCrouch()
{
	// Crouch function
	Crouch();

	// Set Nav Agent property
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	cameraComp->SetRelativeLocation( FVector( -9.492332, 32.671730, 90 ) );
}

void ACharacterManager::EndCrouch()
{
	// UnCrouch function
	UnCrouch();
	cameraComp->SetRelativeLocation( FVector( -9.492332, 32.671730, 162.198380 ) );
}

void ACharacterManager::AimIn()
{
	// Deactivate Camera
	cameraComp->Deactivate();
}

void ACharacterManager::AimOut()
{
	// Activate Camera
	cameraComp->Activate();
}


void ACharacterManager::SetHoldADS( bool holdADS )
{
}

bool ACharacterManager::GetHoldADS()
{
	return m_holdADS;
}

void ACharacterManager::Shoot()
{
	if( projectileClass )
	{
		FVector cameraLocation;
		FRotator cameraRotation;
		GetActorEyesViewPoint( cameraLocation, cameraRotation );

		// MuzzleOffset
		muzzleOffset.Set( 100.0f, 0.0f, 0.0f );

		// Transform Muzzleoffset from camera space to world space
		FVector muzzleLocation = cameraLocation + FTransform( cameraRotation ).TransformVector( muzzleOffset );

		// Skew aim to be upwards
		FRotator muzzleRotation = cameraRotation;
		muzzleRotation.Pitch += 3.0f;

		UWorld* world = GetWorld();
		if( world )
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();

			// Spawn projectile
			AProjectileManager* projectile = world->SpawnActor<AProjectileManager>( projectileClass, muzzleLocation, muzzleRotation, spawnParams );

			if( projectile )
			{
				FVector launchDirection = muzzleRotation.Vector();
				projectile->ShootInDirection( launchDirection );
			}
		}
	}
}