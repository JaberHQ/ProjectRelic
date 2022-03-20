// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPlayer.h"

ACharacterPlayer::ACharacterPlayer()
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	camera = CreateDefaultSubobject<UCameraComponent>( TEXT( "Camera" ) );
	camera->AttachTo( RootComponent );
	camera->SetRelativeLocation( FVector( 0, 0, 40 ) );

}

void ACharacterPlayer::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	PlayerInputComponent->BindAxis( "MoveForward", this, &ACharacterPlayer::moveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACharacterPlayer::moveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &ACharacterPlayer::turn );
	PlayerInputComponent->BindAxis( "LookUp", this, &ACharacterPlayer::lookUp);
}

void ACharacterPlayer::moveForward( float inputAxis )
{
	if( inputAxis )
	{
		AddMovementInput( GetActorForwardVector(), inputAxis );
	}
}

void ACharacterPlayer::moveRight( float inputAxis )
{
	if( inputAxis )
	{
		AddMovementInput( GetActorRightVector(), inputAxis );
	}
}

void ACharacterPlayer::turn( float inputAxis )
{
	AddActorLocalRotation( FRotator( 0, inputAxis, 0 ) );
}

void ACharacterPlayer::lookUp( float inputAxis )
{
	if( inputAxis )
	{
		float temp = camera->GetRelativeRotation().Pitch + inputAxis;
		if( temp <65 && temp > -65 )
		{
			camera->AddLocalRotation( FRotator( inputAxis, 0, 0 ) );
		}
	}
}
