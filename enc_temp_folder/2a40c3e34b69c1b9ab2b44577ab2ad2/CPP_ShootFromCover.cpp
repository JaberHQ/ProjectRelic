// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ShootFromCover.h"
#include "CPP_AIController.h"
#include "CPP_AIManager.h"
#include "Engine/World.h"
EBTNodeResult::Type UCPP_ShootFromCover::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{

		// Use index to get patrol path
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			controllerAI->PlayerHasShot();
			managerAI->SetHasCaughtPlayer( true );
			managerAI->UnCrouch();
			managerAI->ShootProjectile();
			managerAI->TimeToShoot();
			
			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}



