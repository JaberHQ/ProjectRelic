// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_FindPatrolPathTask.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIManager.h"

UCPP_FindPatrolPathTask::UCPP_FindPatrolPathTask( FObjectInitializer const& objectInitalizer )
{
	NodeName = TEXT( "Find Patrol Path Point" );
}

EBTNodeResult::Type UCPP_FindPatrolPathTask::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{

	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = controllerAI->GetBlackboardComp();

		// Get index
		int index = controllerAI->GetBlackboardComp()->GetValueAsInt( "PatrolPathIndex" );

		// Use index to get patrol path
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			FVector point = managerAI->GetPatrolPath()->GetPatrolPoint( index );

			// Transform point to global position
			FVector globalPoint = managerAI->GetPatrolPath()->GetActorTransform().TransformPosition( point );
			controllerAI->GetBlackboardComp()->SetValueAsVector( "PatrolPathVector", globalPoint );

			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
