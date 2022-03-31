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
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>( pawn );

	if( EnemyCharacter )
	{
		if( EnemyCharacter->BehaviourTree->BlackboardAsset )
		{
			BlackboardComp->InitializeBlackboard( *( EnemyCharacter->BehaviourTree->BlackboardAsset ) );
		}

		// Populate patrol point array
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AEnemyPatrolPoint::StaticClass(), patrolPoints );

		BehaviourComp->StartTree( *EnemyCharacter->BehaviourTree );
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

