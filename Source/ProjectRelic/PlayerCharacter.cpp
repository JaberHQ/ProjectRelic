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
	GetMesh()->SetRelativeLocationAndRotation( FVector( 0.0f, 0.0f, -90.0f ), FQuat( FRotator( 0.0f, -90.0f, 0.0f ) ) );

	// Attatch your class Components to the default Skeletal Mesh Component 
	SpringArmComp->SetupAttachment( GetMesh() );
	CameraComp->SetupAttachment( SpringArmComp, USpringArmComponent::SocketName );

	// Setting class variables of the spring arm
	SpringArmComp->bUsePawnControlRotation = true;

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
	// -- Note its not the component --
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::endSprint()
{
	// -- Note its not the component --
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::beginCrouch()
{
	
	Crouch();
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

void APlayerCharacter::endCrouch()
{
	UnCrouch();
	
}

void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( "MoveForward", this, &APlayerCharacter::moveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &APlayerCharacter::moveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );
	PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &APlayerCharacter::beginCrouch );
	PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &APlayerCharacter::endCrouch );
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &APlayerCharacter::beginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &APlayerCharacter::endSprint );
}


