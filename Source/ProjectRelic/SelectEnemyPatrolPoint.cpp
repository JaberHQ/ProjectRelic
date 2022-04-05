// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectEnemyPatrolPoint.h"


EBTNodeResult::Type USelectEnemyPatrolPoint::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	// Get ai controller
	AEnemyController* enemyController = Cast<AEnemyController>( ownerComp.GetAIOwner() );

	if( enemyController )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = enemyController->GetBlackboardComp();

		// Get the current point of the enemy trail
		AEnemyPatrolPoint* currentPoint = Cast<AEnemyPatrolPoint>( blackboardComp->GetValueAsObject( "locationToGo" ) );

		// Array of the available patrol points
		TArray<AActor*> availablePatrolPoints = enemyController->GetPatrolPoints();

		// Set the next patrol point to null
		AEnemyPatrolPoint* nextPatrolPoint = nullptr;

		// If patrol point is not the final point
		if( enemyController->currentPatrolPoint != availablePatrolPoints.Num() - 1 )
		{
			// Increase index
			nextPatrolPoint = Cast<AEnemyPatrolPoint>( availablePatrolPoints[ ++enemyController->currentPatrolPoint ] );
		}
		else
		{
			// Else reset index back to 0
			nextPatrolPoint = Cast<AEnemyPatrolPoint>( availablePatrolPoints[ 0 ] );
			enemyController->currentPatrolPoint = 0;
		}
		// Set BB keys
		blackboardComp->SetValueAsObject( "locationToGo", nextPatrolPoint );

		// Return node has succeeded
		return EBTNodeResult::Succeeded;
	}
	// Return node has failed
	return EBTNodeResult::Failed;
}
