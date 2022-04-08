// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyController.h"


AEnemyController::AEnemyController()
{
	
	// BT and BB
	behaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// BB keys
	locationToGoKey = "locationToGo";
	playerKey = "target";
	hasLineOfSight = "hasLineOfSight";
	currentPatrolPoint = 0;

	
}

AEnemyController::~AEnemyController()
{

}

void AEnemyController::SetPlayerCaught( const TArray<AActor*>& CaughtActors )
{
	if( blackboardComp )
	{
		blackboardComp->SetValueAsBool( hasLineOfSight, true );
		blackboardComp->SetValueAsObject( playerKey, CaughtActors[0]);
	}
}

void AEnemyController::SetHasLineOfSight( bool boolean )
{
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

