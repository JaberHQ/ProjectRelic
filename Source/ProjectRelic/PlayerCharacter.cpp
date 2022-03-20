// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacter::APlayerCharacter()
{
	// Instantiating class components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	CameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );

	// Set the location and rotation of the Characrer Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation( FVector( -10.0f, 20.0f, 160.0f ), FQuat( FRotator( 0.0f, 0.0, 90.0f ) ) );

	// Attatch your class Components to the default Skeletal Mesh Component 
	//SpringArmComp->SetupAttachment( CameraComp, UCameraComponent:: );
	CameraComp->SetupAttachment( GetMesh() );

	// Setting class variables of the spring arm
	//SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = true;
	// Setting class variables of the Character movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

}

void APlayerCharacter::moveForward( float inputAxis )
{
	if( ( Controller != nullptr )  && (inputAxis != 0.0f ) )
	{
		// Find out the forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
		AddMovementInput( Direction, inputAxis );
	}
}

void APlayerCharacter::moveRight( float inputAxis )
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

void APlayerCharacter::beginSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::endSprint()
{
	// Set speed
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::beginCrouch()
{
	// Crouch function
	Crouch();

	// Set Nav Agent property
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Set camera location
	CameraComp->SetRelativeLocation( FVector( -10, 20, 100 ) );
}

void APlayerCharacter::endCrouch()
{
	// UnCrouch function
	UnCrouch();

	// Reset camera
	CameraComp->SetRelativeLocation( FVector( -10, 20, 160 ) );
}

void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	// Movement
	PlayerInputComponent->BindAxis( "MoveForward", this, &APlayerCharacter::moveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &APlayerCharacter::moveRight );

	// Mouse rotation
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	// Jump
	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );

	// Crouch
	PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &APlayerCharacter::beginCrouch );
	PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &APlayerCharacter::endCrouch );

	// Sprint
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &APlayerCharacter::beginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &APlayerCharacter::endSprint );
}


