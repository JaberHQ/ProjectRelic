// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIController.h"
#include "CPP_AIManager.h"
#include "CPP_PlayerManager.h"

ACPP_AIController::ACPP_AIController()
{
	// BT and BB
	behaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>( TEXT( "BehaviourComp" ) );
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>( TEXT( "BlackboardComp" ) );

	// Set variables to the BB keys
	m_patrolLocation = "PatrolLocation";
	m_enemyActor = "EnemyActor";
	m_hasLineOfSight = "HasLineOfSight";
	m_patrolPathVector = "PatrolPathVector";
	m_patrolPathIndex = "PatrolPathIndex";
	m_lastKnownLocation = "LastKnownLocation";
	m_invesigate = "Investigate";
}

void ACPP_AIController::OnPossess( APawn* pawn )
{
	Super::OnPossess( pawn );

	// Reference to AI Pawn
	ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( pawn );
	if( managerAI )
	{
		if( managerAI->behaviourTree->BlackboardAsset )
		{
			// Initalise BB
			blackboardComp->InitializeBlackboard( *( managerAI->behaviourTree->BlackboardAsset ) );
		}
		// Initalise BT
		behaviourComp->StartTree( *managerAI->behaviourTree );
	}
}

UBlackboardComponent* ACPP_AIController::GetBlackboardComp() const
{
	return blackboardComp;
}


void ACPP_AIController::SetPlayerCaught( const TArray<AActor*>& caughtActors )
{
	if( blackboardComp )
	{
		// Set bool to true and set target to be the Player
		blackboardComp->SetValueAsBool( m_hasLineOfSight, true );
		blackboardComp->SetValueAsObject( m_enemyActor, caughtActors[ 0 ] );
	}
}

void ACPP_AIController::SetHasLineOfSight( bool boolean )
{
	if( blackboardComp )
	{
		// Set bool
		blackboardComp->SetValueAsBool( m_hasLineOfSight, boolean );
	}
}

bool ACPP_AIController::GetHasLineOfSight()
{
	
	// Return bool
	return blackboardComp->GetValueAsBool( m_hasLineOfSight );
			
	
}

void ACPP_AIController::SetLastKnownLocation( FVector vector )
{
	if( blackboardComp )
	{
		blackboardComp->SetValueAsVector( m_lastKnownLocation, vector );
	}
}

void ACPP_AIController::SetInvestigate( bool boolean )
{
	if( blackboardComp )
	{
		// Set bool
		blackboardComp->SetValueAsBool( m_invesigate, boolean );

	}
}

FVector ACPP_AIController::GetLastKnownLocation()
{
	
	return blackboardComp->GetValueAsVector( m_lastKnownLocation );

}

void ACPP_AIController::PlayerHasShot()
{
	if( blackboardComp )
	{
		ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );
		if( playerManager )
		{
			// Set bool to true and set target to be the Player
			blackboardComp->SetValueAsBool( m_hasLineOfSight, true );
			blackboardComp->SetValueAsObject( m_enemyActor, playerManager );

			//SFX
		}
	}
}
