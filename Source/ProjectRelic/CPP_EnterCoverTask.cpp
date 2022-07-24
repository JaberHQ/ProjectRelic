// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnterCoverTask.h"
#include "CPP_AIController.h"
#include "CPP_AIManager.h"

EBTNodeResult::Type UCPP_EnterCoverTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{

		// Use index to get patrol path
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			managerAI->EnterCover();

			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );

			controllerAI->SetInCover( true );

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
