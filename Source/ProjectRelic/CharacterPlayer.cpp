// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"

void ACharacterPlayer::moveForward( float inputAxis )
{
	if( inputAxis != 0.0f )
	{
		// add movement in that direction
		AddMovementInput( GetActorForwardVector(), inputAxis );
	}
}

void ACharacterPlayer::moveRight( float inputAxis )
{
	if( inputAxis != 0.0f )
	{
		// add movement in that direction
		AddMovementInput( GetActorRightVector(), inputAxis );
	}
}

void ACharacterPlayer::turnRate( float rate )
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput( rate * baseTurnRate * GetWorld()->GetDeltaSeconds() );
}

void ACharacterPlayer::lookUpRate( float rate )
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput( rate * baseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

ACharacterPlayer::ACharacterPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize( 55.f, 96.0f );
	
	// set our turn rates for input
	baseTurnRate = 45.f;
	baseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "FirstPersonCamera" ) );
	FirstPersonCameraComponent->SetupAttachment( GetCapsuleComponent() );
	FirstPersonCameraComponent->SetRelativeLocation( FVector( -39.56f, 1.75f, 64.f ) ); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "CharacterMesh1P" ) );
	Mesh1P->SetOnlyOwnerSee( true );
	Mesh1P->SetupAttachment( FirstPersonCameraComponent );
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation( FRotator( 1.9f, -19.19f, 5.2f ) );
	Mesh1P->SetRelativeLocation( FVector( -0.5f, -4.4f, -155.7f ) );
}

void ACharacterPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	// set up gameplay key bindings
	check( PlayerInputComponent );

	// Bind jump events
	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );

	// Bind movement events
	PlayerInputComponent->BindAxis( "MoveForward", this, &ACharacterPlayer::moveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACharacterPlayer::moveRight );

	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "TurnRate", this, &ACharacterPlayer::turnRate );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );
	PlayerInputComponent->BindAxis( "LookUpRate", this, &ACharacterPlayer::lookUpRate );
}


