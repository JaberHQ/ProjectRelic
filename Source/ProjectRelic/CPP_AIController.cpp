// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIController.h"
#include "CPP_AIManager.h"

ACPP_AIController::ACPP_AIController()
{
	// BT and BB
	behaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// Set variables to the BB keys
	patrolLocation = "PatrolLocation";
	enemyActor = "EnemyActor";
	hasLineOfSight = "HasLineOfSight";
	patrolPathVector = "PatrolPathVector";
	patrolPathIndex = "PatrolPathIndex";
}

void ACPP_AIController::OnPossess( APawn* pawn )
{
	Super::OnPossess( pawn );

	ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( pawn );
	if( managerAI )
	{
		if( managerAI->behaviourTree->BlackboardAsset )
		{
			blackboardComp->InitializeBlackboard( *( managerAI->behaviourTree->BlackboardAsset ) );
		}

		behaviourComp->StartTree( *managerAI->behaviourTree );
	}
}

UBlackboardComponent* ACPP_AIController::GetBlackboardComp() const
{
	return blackboardComp;
}
