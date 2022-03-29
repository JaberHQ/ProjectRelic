// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterManager::ACharacterManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating class components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	CameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	ADSCameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "ADSCameraComp" ) );

	// Set the location and rotation of the Characrer Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation( FVector( -6.0f, 24.0f, 130.0f ), FQuat( FRotator( 0.0f, 0.0, 90.0f ) ) );

	// Attatch your class Components to the default Skeletal Mesh Component 
	CameraComp->SetupAttachment( GetMesh() );
	CameraComp->AttachTo( GetMesh(), TEXT( "head" ), EAttachLocation::SnapToTargetIncludingScale, true );
	CameraComp->SetRelativeLocation( FVector( 0.0f, 33.0f, 160.0f ) );
	CameraComp->SetRelativeRotation( FRotator( 0.0f, 90.0f, 0.0f ) );

	CameraComp->bUsePawnControlRotation = false;

	GunComp->SetupAttachment( GetMesh() );
	ADSCameraComp->SetupAttachment( GunComp );
	GunComp->AttachTo( GetMesh(), TEXT( "thumb_01_l" ), EAttachLocation::SnapToTargetIncludingScale, true );

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
void ACharacterManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	// Movement
	PlayerInputComponent->BindAxis( "MoveForward", this, &ACharacterManager::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACharacterManager::MoveRight );

	// Mouse rotation
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	// Jump
	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );

	// Crouch
	PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACharacterManager::BeginCrouch );
	PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &ACharacterManager::EndCrouch );

	// Sprint
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACharacterManager::BeginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &ACharacterManager::EndSprint );

	// Aim
	PlayerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACharacterManager::AimIn );
	PlayerInputComponent->BindAction( "Aim", IE_Released, this, &ACharacterManager::AimOut );

	// Shoot
	PlayerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACharacterManager::Shoot );
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


}

void ACharacterManager::EndCrouch()
{
	// UnCrouch function
	UnCrouch();

}

void ACharacterManager::AimIn()
{
	//CameraComp->SetRelativeLocation( FVector( -15, 20, 165 ) );
	/*if( m_holdADS == false )
	{
		isAimedIn = true;
		CameraComp->SetFieldOfView( 70.0f );
	}*/
	CameraComp->Deactivate();

}

void ACharacterManager::AimOut()
{
	// Reset camera
	//CameraComp->SetRelativeLocation( FVector( -6.0f, 24.0f, 130.0f ) );
	/*if( m_holdADS == false )
	{
		isAimedIn = false;
		CameraComp->SetFieldOfView( 90.0f );
	}*/
	CameraComp->Activate();

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
	if( ProjectileClass )
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint( CameraLocation, CameraRotation );

		// MuzzleOffset
		MuzzleOffset.Set( 100.0f, 0.0f, 0.0f );

		// Transform Muzzleoffset from camera space to world space
		FVector MuzzleLocation = CameraLocation + FTransform( CameraRotation ).TransformVector( MuzzleOffset );

		// Skew aim to be upwards
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 3.0f;

		UWorld* World = GetWorld();
		if( World )
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn projectile
			AProjectileManager* Projectile = World->SpawnActor<AProjectileManager>( ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams );

			if( Projectile )
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->ShootInDirection( LaunchDirection );
			}
		}
	}
}