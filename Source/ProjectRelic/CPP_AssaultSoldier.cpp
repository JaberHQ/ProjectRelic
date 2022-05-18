// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AssaultSoldier.h"

ACPP_AssaultSoldier::ACPP_AssaultSoldier()
:m_defaultWalkSpeed( 300.0f )
{
	// Set default walk speed
	UpdateWalkSpeed( m_defaultWalkSpeed );
}

void ACPP_AssaultSoldier::BeginPlay()
{
	Super::BeginPlay();
}
