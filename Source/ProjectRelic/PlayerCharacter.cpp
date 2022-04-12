// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"


APlayerCharacter::APlayerCharacter()
{
	
	
}

void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* playerInputComponent )
{
	Super::SetupPlayerInputComponent( playerInputComponent );

	// Takedown
	playerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &APlayerCharacter::Takedown );
}

void APlayerCharacter::Takedown()
{
	// Debug message
	GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Takedown" ) ) );
}




