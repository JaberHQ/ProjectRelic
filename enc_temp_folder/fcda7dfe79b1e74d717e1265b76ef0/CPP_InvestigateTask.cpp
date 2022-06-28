// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InvestigateTask.h"
#include "CPP_AIController.h"
EBTNodeResult::Type UCPP_InvestigateTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );
	if( controllerAI )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = controllerAI->GetBlackboardComp();

		// Get index
		bool investigate = controllerAI->GetBlackboardComp()->GetValueAsBool( "Investigate" );

		if( investigate )
		{
			controllerAI->MoveTo( controllerAI->GetLastKnownLocation() );
			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}


	return EBTNodeResult::Failed;
}
