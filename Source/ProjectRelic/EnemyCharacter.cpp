// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"


AEnemyCharacter::AEnemyCharacter()
{
}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::UpdateWalkSpeed( float ChaseSpeed )
{
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}
