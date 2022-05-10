// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerManager.h"

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Start player off as crouching
	Crouch();
}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

}


