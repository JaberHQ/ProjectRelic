// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ShootTask.h"
#include "CPP_AIController.h"
#include "CPP_AIManager.h"

EBTNodeResult::Type UCPP_ShootTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			managerAI->ShootProjectile();

			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
