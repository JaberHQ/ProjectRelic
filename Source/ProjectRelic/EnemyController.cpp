// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"
#include "EnemyCharacter.h"
#include "EnemyPatrolPoint.h"
#include <Engine.h>




AEnemyController::AEnemyController()
{
	
	// BT and BB
	BehaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// BB keys
	locationToGoKey = "locationToGo";
	playerKey = "target";
	currentPatrolPoint = 0;
}

AEnemyController::~AEnemyController()
{
	
}

void AEnemyController::SetPlayerCaught( APawn* pawn )
{
	if( BlackboardComp )
	{
		BlackboardComp->SetValueAsObject( playerKey, pawn );
	}
}

void AEnemyController::OnPossess( APawn* pawn )
{
	Super::Possess( pawn );

	// Get ref to character
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );

	if( enemyCharacter )
	{
		if( enemyCharacter->BehaviourTree->BlackboardAsset )
		{
			BlackboardComp->InitializeBlackboard( *( enemyCharacter->BehaviourTree->BlackboardAsset ) );
		}

		// Populate patrol point array
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AEnemyPatrolPoint::StaticClass(), patrolPoints );

		BehaviourComp->StartTree( *enemyCharacter->BehaviourTree );
	}
}

UBlackboardComponent* AEnemyController::GetBlackboardComp() const
{
	return BlackboardComp;
}

TArray<AActor*> AEnemyController::GetPatrolPoints() const
{
	return patrolPoints;
}

