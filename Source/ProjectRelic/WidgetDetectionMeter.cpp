// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDetectionMeter.h"
#include "EnemyCharacter.h"
#include "GameFramework/Pawn.h"

void UWidgetDetectionMeter::Construct( APawn* pawn )
{
	Super::Construct();

	// Reference to player controller
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );

	
}
