// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

AEnemyController::AEnemyController()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComponent" ) );
}

AEnemyController::~AEnemyController()
{
}

void AEnemyController::onTargetPerceptionUpdated()
{
	if( AActor().ActorHasTag( TEXT( "Player" ) )/*&& Break Stimulus is sensed*/ )
	{
		// Clear Timer
		// lineOfSight = true;
		// target enemyActor

	}
	else
	{
		// Set timer
		// lineOfSight = false;
	}
}
