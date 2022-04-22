// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDetectionMeter.h"


void UWidgetDetectionMeter::Construct( bool isDesignTime, const TArray<AActor*>& caughtActors )
{
	Super::PreConstruct( isDesignTime );

	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( caughtActors[0]);

	if( sightRegisteredDelegate.IsBound() )
	{
		sightRegisteredDelegate.Broadcast( m_hasBeenSeen, m_detectionSpeed, caughtActors );
	}
	
}

void UWidgetDetectionMeter::RespondToDetection( bool hasBeenSeen, float detectionSpeed, const TArray<AActor*>& caughtActors )
{
	m_hasBeenSeen = hasBeenSeen;
	m_detectionSpeed = detectionSpeed;
	
	// Set visiblity to true
}


