// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"

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
	Super::BeginPlay();

	if( pawnSensingComp )
	{
		pawnSensingComp->OnSeePawn.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
	}
}

AEnemyCharacter* AEnemyCharacter::GetEnemyCharacter( APawn* pawn ) const
{
	AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );
	return enemyCharacter;
}

void AEnemyCharacter::OnPlayerCaught( APawn* pawn )
{
	// get ref to player controller
	AEnemyController* enemycontroller = Cast<AEnemyController>( GetController() );

	if( enemycontroller )
	{
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "you've been caught!" ) ) );
		enemycontroller->SetPlayerCaught( pawn );
	}
}