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
	m_move = "Move";
	m_inCover = "InCover";
	m_bottleLocation = "BottleLocation";
	m_dead = "Dead";
}

void ACPP_AIController::OnPossess( APawn* pawn )
{
	Super::OnPossess( pawn );

	// Cast to AI
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
	// Get BB
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
	// Get bool
	return blackboardComp->GetValueAsBool( m_hasLineOfSight );	
}

void ACPP_AIController::SetLastKnownLocation( FVector vector )
{
	if( blackboardComp )
	{
		// Set vector
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
			
		}
	}
}

FVector ACPP_AIController::GetMove()
{
	return blackboardComp->GetValueAsVector( m_move );
}

void ACPP_AIController::SetInCover( bool boolean )
{
	blackboardComp->SetValueAsBool( m_inCover, boolean );
}

bool ACPP_AIController::GetInCover() const
{
	return blackboardComp->GetValueAsBool( m_inCover );
}

void ACPP_AIController::SetBottleLocation( FVector bottleLocation )
{
	blackboardComp->SetValueAsVector( m_bottleLocation, bottleLocation );
}

FVector ACPP_AIController::GetBottleLocation()
{
	return blackboardComp->GetValueAsVector( m_bottleLocation );
}

void ACPP_AIController::SetDead( bool boolean )
{
	blackboardComp->SetValueAsBool( m_dead, boolean );
}

bool ACPP_AIController::GetDead()
{
	return blackboardComp->GetValueAsBool( m_dead );
}
