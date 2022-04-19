// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"


AEnemyController::AEnemyController()
	:currentPatrolPoint( 0 )
{
	// BT and BB
	behaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// Set variables to the BB keys
	locationToGoKey = "locationToGo";
	playerKey = "target";
	hasLineOfSight = "hasLineOfSight";

}

AEnemyController::~AEnemyController()
{
}

void AEnemyController::SetPlayerCaught( const TArray<AActor*>& caughtActors )
{
	if( blackboardComp )
	{
		// Set bool to true and set target to be the Player
		blackboardComp->SetValueAsBool( hasLineOfSight, true );
		blackboardComp->SetValueAsObject( playerKey, caughtActors[ 0 ]);
	}
}

void AEnemyController::SetHasLineOfSight( bool boolean )
{
	// Set boolean
	blackboardComp->SetValueAsBool( hasLineOfSight, boolean );
}

bool AEnemyController::GetHasLineOfSight()
{
	return blackboardComp->GetValueAsBool( hasLineOfSight );
}


void AEnemyController::OnPossess( APawn* pawn )
{
	Super::OnPossess( pawn );

	// Get ref to character
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );
	if( enemyCharacter )
	{
		if( enemyCharacter->behaviourTree->BlackboardAsset )
		{
			// Initialise BB
			blackboardComp->InitializeBlackboard( *( enemyCharacter->behaviourTree->BlackboardAsset ) );
		}

		// Populate patrol point array
		UGameplayStatics::GetAllActorsOfClass( GetWorld(), AEnemyPatrolPoint::StaticClass(), patrolPoints );

		// Initialise BT
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



