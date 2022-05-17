// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIController.h"

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

UBlackboardComponent* ACPP_AIController::GetBlackboardComp() const
{
	return blackboardComp;
}
