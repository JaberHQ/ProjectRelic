// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectEnemyPatrolPoint.h"
#include "EnemyPatrolPoint.h"
#include "EnemyController.h"

EBTNodeResult::Type USelectEnemyPatrolPoint::ExecuteTask( UBehaviorTreeComponent& ownerComp, uint8* nodeMemory )
{
	AEnemyController* enemyController = Cast<AEnemyController>( ownerComp.GetAIOwner() );

	if( enemyController )
	{
		// Get BB
		UBlackboardComponent* blackboardComp = enemyController->GetBlackboardComp();

		AEnemyPatrolPoint* currentPoint = Cast<AEnemyPatrolPoint>( blackboardComp->GetValueAsObject( "locationToGo" ) );

		TArray<AActor*> availablePatrolPoints = enemyController->GetPatrolPoints();

		AEnemyPatrolPoint* nextPatrolPoint = nullptr;

		if( enemyController->currentPatrolPoint != availablePatrolPoints.Num() - 1 )
		{
			nextPatrolPoint = Cast<AEnemyPatrolPoint>( availablePatrolPoints[ ++enemyController->currentPatrolPoint ] );
		}
		else
		{
			nextPatrolPoint = Cast<AEnemyPatrolPoint>( availablePatrolPoints[ 0 ] );
			enemyController->currentPatrolPoint = 0;
		}
		blackboardComp->SetValueAsObject( "locationToGo", nextPatrolPoint );

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
