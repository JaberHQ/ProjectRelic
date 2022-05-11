// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ChasePlayerTask.h"

EBTNodeResult::Type UCPP_ChasePlayerTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* enemyController = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( enemyController )
	{
		// Return node has succeeded
		return EBTNodeResult::Succeeded;
	}
	// Return node has failed
	return EBTNodeResult::Failed;
}
