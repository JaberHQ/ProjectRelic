// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
{
	//Initialise senses
	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "PawnSensingComp" ) );
	pawnSensingComp->SetPeripheralVisionAngle( 90.0f );

}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::UpdateWalkSpeed( float chaseSpeed )
{
	GetCharacterMovement()->MaxWalkSpeed = chaseSpeed;
}

void AEnemyCharacter::BeginPlay()
{
	if( pawnSensingComp )
	{
		pawnSensingComp->OnSeePawn.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
	}
}

void AEnemyCharacter::OnPlayerCaught( APawn* pawn )
{
	// Get ref to player controller
	AEnemyController* enemyController = Cast<AEnemyController>( GetController() );

	if( enemyController )
	{
		//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "You've been caught!" ) ) );
		enemyController->SetPlayerCaught( pawn );
	}
}