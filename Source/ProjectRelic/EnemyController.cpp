// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"




AEnemyController::AEnemyController()
	:enemyCharacter( nullptr )
	,currentPatrolPoint( 0 )
	,locationToGoKey( "locationToGo" )
	,playerKey( "target" )
{
	
	// BT and BB
	behaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// BB keys
	/*locationToGoKey = "locationToGo";
	playerKey = "target";*/
	//currentPatrolPoint = 0;
}

AEnemyController::~AEnemyController()
{
	delete enemyCharacter;
	enemyCharacter = nullptr;

}

void AEnemyController::SetPlayerCaught( APawn* pawn )
{
	if( blackboardComp )
	{
		blackboardComp->SetValueAsObject( playerKey, pawn );
	}
}

void AEnemyController::OnPossess( APawn* pawn )
{
	Super::Possess( pawn );

	// Get ref to character
	enemyCharacter = enemyCharacter->GetEnemyCharacter( pawn );
	if( enemyCharacter )
	{
		if( enemyCharacter->behaviourTree->BlackboardAsset )
		{
			blackboardComp->InitializeBlackboard( *( enemyCharacter->behaviourTree->BlackboardAsset ) );
		}

		// Populate patrol point array
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AEnemyPatrolPoint::StaticClass(), patrolPoints );

		behaviourComp->StartTree( *enemyCharacter->behaviourTree );
	}
}

UBlackboardComponent* AEnemyController::GetBlackboardComp() const
{
	return blackboardComp;
}

TArray<AActor*> AEnemyController::GetPatrolPoints() const
{
	return patrolPoints;
}

