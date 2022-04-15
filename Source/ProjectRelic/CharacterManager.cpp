// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterManager.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACharacterManager::ACharacterManager()
	:m_holdADS( false )
	,m_muzzleRotationPitch( 3.0f )
	,m_walkSpeed( 600.0f )
	,m_sprintSpeed( 1000.0f )
	,m_crouchSpeed( 300.0f )
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating class components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	aDSCameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "ADSCameraComp" ) );

	// Set the location and rotation of the Characrer Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation( FVector( 0.0f, 0.0f, -90.0f ), FQuat( FRotator( 0.0f, -90.0f, 0.0f ) ) );

	// Attatch your class Components to the default Skeletal Mesh Component 


	springArmComp->SetupAttachment( GetMesh() );
	springArmComp->bUsePawnControlRotation = true;

	cameraComp->AttachTo( springArmComp, USpringArmComponent::SocketName );

	// Gun
	gunComp->SetupAttachment( GetMesh() );
	gunComp->GetSocketBoneName( TEXT( "RightHandThumb1" ) );
	
	// ADS Camera
	aDSCameraComp->SetupAttachment( gunComp );
	aDSCameraComp->Deactivate();

	// Setting class variables of the Character movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	// Set Nav Agent property
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	
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

void ACharacterManager::LineTrace()
{
	// Line tracing
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint( Loc, Rot );
	FVector Start = Loc;
	FVector End = Start + ( Rot.Vector() * 2000.0f );

	FCollisionQueryParams TraceParams;
	GetWorld()->LineTraceSingleByChannel( Hit, Start, End, ECC_Visibility, TraceParams );

	DrawDebugLine( GetWorld(), Start, End, FColor::Red, false, 2.0f );


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

void ACharacterManager::Takedown()
{
	// Moved to player
}

void ACharacterManager::BeginSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = m_sprintSpeed;
}

void ACharacterManager::EndSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = m_walkSpeed;
}

void ACharacterManager::BeginCrouch()
{
	// Crouch function
	Crouch();

}

void ACharacterManager::EndCrouch()
{
	// UnCrouch function
	UnCrouch();

}

void ACharacterManager::AimIn()
{
	// Deactivate Camera
	cameraComp->Deactivate();
	aDSCameraComp->Activate();
}

void ACharacterManager::AimOut()
{
	// Activate Camera
	cameraComp->Activate();
	aDSCameraComp->Deactivate();
}

void ACharacterManager::UpdateWalkSpeed( float speed )
{
	// Set walk speed
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void ACharacterManager::TakedownTrace()
{

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

		// Muzzle offset
		muzzleOffset.Set( 100.0f, 0.0f, 0.0f );

		// Transform Muzzleoffset from camera space to world space
		FVector muzzleLocation = cameraLocation + FTransform( cameraRotation ).TransformVector( muzzleOffset );

		// Skew aim to be upwards
		FRotator muzzleRotation = cameraRotation;
		muzzleRotation.Pitch += m_muzzleRotationPitch;

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