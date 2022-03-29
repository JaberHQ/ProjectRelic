// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "EnemyManager.h"
#include "EnemyPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AEnemyController::AEnemyController()
{
	/*PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComp" ) );
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "AISightConfig" ) );*/

	BehaviourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourTreeComp" ) );
	BackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BackboardComp" ) );

	PlayerKey = "target";
	PatrolLocationKey = "patrolLocation";
}

AEnemyController::~AEnemyController()
{
}

void AEnemyController::OnTargetPerceptionUpdated()
{
	//if( ActorHasTag( TEXT( "Player" ) ) /*&& SightConfig->*/ )
	//{

	//}
}

void AEnemyController::OnPossess( APawn* Pawn )
{
	Super::Possess( Pawn );

	//Get Ref
	AEnemyManager* AIEnemy = Cast<AEnemyManager>( Pawn );

	if( AIEnemy )
	{
		if( AIEnemy->BehaviourTree->BlackboardAsset )
		{

		}
	}
}
