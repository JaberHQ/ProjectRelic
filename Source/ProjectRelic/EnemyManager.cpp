// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyManager::AEnemyManager()
{
	BehaviourTree = CreateDefaultSubobject<UBehaviorTree>( TEXT( "BehaviourTree" ) );
}

AEnemyManager::~AEnemyManager()
{

}

void AEnemyManager::UpdateWalkSpeed( float ChaseSpeed )
{
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}
