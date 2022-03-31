// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include <Runtime/Engine/Private/GameplayStatics.cpp>

AEnemyController::AEnemyController()
{
	
	// BT and BB
	BehaviourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourTreeComp" ) );
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// BB keys
	locationToGoKey = "locationToGo";
	playerKey = "target";
}

AEnemyController::~AEnemyController()
{
}

void AEnemyController::SetPlayerCaught( APawn* Pawn )
{
}

void AEnemyController::OnPossess( APawn* Pawn )
{
	Super::Possess( Pawn );

	// Get ref to character
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>( Pawn );

	if( EnemyCharacter )
	{
		if( EnemyCharacter->BehaviourTree->BlackboardAsset )
		{
			BlackboardComp->InitializeBlackboard( *( EnemyCharacter->BehaviourTree->BlackboardAsset ) );
		}

		// Populate patrol point array
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AEnemyPatrolPoint::StaticClass(), patrolPoints );

		BehaviourTreeComp->StartTree( *EnemyCharacter->BehaviourTree );
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

