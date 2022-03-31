// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
{
	//Initialise senses
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "PawnSensingComp" ) );
	PawnSensingComp->SetPeripheralVisionAngle( 90.0f );

}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::UpdateWalkSpeed( float ChaseSpeed )
{
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
}

void AEnemyCharacter::BeginPlay()
{
	if( PawnSensingComp )
	{
		PawnSensingComp->OnSeePawn.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
	}
}

void AEnemyCharacter::OnPlayerCaught( APawn* Pawn )
{
	// Get ref to player controller
	AEnemyController* EnemyController = Cast<AEnemyController>( GetController() );

	if( EnemyController )
	{
		//GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "You've been caught!" ) ) );
		EnemyController->SetPlayerCaught( Pawn );
	}
}