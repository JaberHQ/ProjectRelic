// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_IncrementPointIndex.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIManager.h"

UCPP_IncrementPointIndex::UCPP_IncrementPointIndex( FObjectInitializer const& objectInitalizer )
{
	NodeName = TEXT( "Increment Path Index" );
}

EBTNodeResult::Type UCPP_IncrementPointIndex::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			int numOfPoints = managerAI->GetPatrolPath()->Num();
			int const minIndex = 0;
			int const maxIndex = numOfPoints - 1;

			//BB index
			int index = controllerAI->GetBlackboardComp()->GetValueAsInt( "PatrolPathIndex" );
			if( index >= maxIndex && direction == EDirectionType::Forward )
			{
				direction = EDirectionType::Reverse;
			}
			else if( index >= minIndex && direction == EDirectionType::Reverse )
			{
				direction = EDirectionType::Forward;
			}
			controllerAI->GetBlackboardComp()->SetValueAsInt( "PatrolPathIndex", ( direction == EDirectionType::Forward ? ++index : -- index) % numOfPoints );
			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
