// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
<<<<<<< HEAD


AEnemyController::AEnemyController()
{
	/*PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComp" ) );
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "AISightConfig" ) );*/

}

AEnemyController::~AEnemyController()
{
}

void AEnemyController::OnTargetPerceptionUpdated()
{
	if( ActorHasTag( TEXT( "Player" ) ) /*&& SightConfig->StimuliSensed*/ )
	{
		//Clear and invialidate timer by handle
		//Has line of sight (blackboard) = true;
		//Enemy Actor (blackboard) = true;
	}
	else
	{
		//Set line of sight timer =4.0f;
		//Set line of sight = false;
		//Set enemy actor = false;
	}
}
=======

>>>>>>> parent of b521974 (WIP Enemy AI C++)
