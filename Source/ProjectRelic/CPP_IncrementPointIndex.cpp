// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_IncrementPointIndex.h"
#include "CPP_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CPP_AIManager.h"

UCPP_IncrementPointIndex::UCPP_IncrementPointIndex( FObjectInitializer const& objectInitalizer )
{
	NodeName = TEXT( "Increment Path Index" ); // Set node name

	m_direction = EDirectionType::Forward; // Set default direction
}

EBTNodeResult::Type UCPP_IncrementPointIndex::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get AI controller
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( ownerComp.GetAIOwner() );

	if( controllerAI )
	{
		// Get AI character
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( controllerAI->GetPawn() );
		if( managerAI )
		{
			// Number of patrol points
			int numOfPoints = managerAI->GetPatrolPath()->Num();

			// Minimum and Maximum index
			int const minIndex = 0;
			int const maxIndex = numOfPoints - 1;

			//BB index
			int index = controllerAI->GetBlackboardComp()->GetValueAsInt( "PatrolPathIndex" );

			// If enemy is going forward and has reached the last patrol point
			if( index >= maxIndex && m_direction == EDirectionType::Forward )
			{
				// Turn around (Go backwards)
				m_direction = EDirectionType::Reverse;
			}
			// If enemy has reached the first patrol point
			else if( index >= minIndex && m_direction == EDirectionType::Reverse )
			{
				// Turn around (Go forwards)
				m_direction = EDirectionType::Forward;
			}

			// Set patrol path index, if going forward is true increment index, otherwise reduce
			controllerAI->GetBlackboardComp()->SetValueAsInt( "PatrolPathIndex", ( m_direction == EDirectionType::Forward ? ++index : -- index) % numOfPoints );

			// Success
			FinishLatentTask( ownerComp, EBTNodeResult::Succeeded );
			return EBTNodeResult::Succeeded;
		}
	}
	// Failed
	return EBTNodeResult::Failed;
}
