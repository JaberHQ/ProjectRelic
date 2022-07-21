// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InvestigateTask.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIManager.h"

EBTNodeResult::Type UCPP_InvestigateTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
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
			FVector point = controllerAI->GetLastKnownLocation();
			controllerAI->GetBlackboardComp()->SetValueAsVector( "LastKnownLocation", point );
			
			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
