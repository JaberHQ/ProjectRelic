// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_FindPatrolPoint.h"

EBTNodeResult::Type UCPP_FindPatrolPoint::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	ACPP_AIController* enemyController = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );
	if( enemyController )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = enemyController->GetBlackboardComp();

		// Return node has succeeded
		return EBTNodeResult::Succeeded;
	}
	// Return node has failed
	return EBTNodeResult::Failed;
}
