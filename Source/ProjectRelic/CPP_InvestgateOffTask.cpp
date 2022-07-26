// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_InvestgateOffTask.h"
#include "CPP_AIController.h"
EBTNodeResult::Type UCPP_InvestgateOffTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		controllerAI->SetInvestigate( false );
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
