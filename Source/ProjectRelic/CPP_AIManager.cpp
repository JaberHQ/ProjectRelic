// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIManager.h"

ACPP_AIManager::ACPP_AIManager()
{
	// Call function to change AI walk speed
	UpdateWalkSpeed( 120.0f );
}

ACPP_PatrolPoint* ACPP_AIManager::GetPatrolPath()
{
	return patrolPath;
}
