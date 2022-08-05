// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ChangeCoverTask.h"
#include "CPP_AIController.h"
EBTNodeResult::Type UCPP_ChangeCoverTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		controllerAI->SetInCover( false );

		FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}