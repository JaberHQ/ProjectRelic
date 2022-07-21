// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GetIntoCoverTask.h"
#include "CPP_AIController.h"
#include "CPP_AIManager.h"
EBTNodeResult::Type UCPP_GetIntoCoverTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{

	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = controllerAI->GetBlackboardComp();

		// Use index to get patrol path
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			controllerAI->MoveToLocation( controllerAI->GetMove() );
			
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );

			managerAI->EnterCover();
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;

}
