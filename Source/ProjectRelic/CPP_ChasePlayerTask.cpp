// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ChasePlayerTask.h"

EBTNodeResult::Type UCPP_ChasePlayerTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		// Get AI manager
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			// Speed up enemy
			managerAI->UpdateWalkSpeed( 600.0f );

			// Return node has succeeded
			return EBTNodeResult::Succeeded;
		}
	}
	// Return node has failed
	return EBTNodeResult::Failed;
}
