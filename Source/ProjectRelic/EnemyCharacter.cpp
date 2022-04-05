// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyController.h"

//#include "Engine.h".

AEnemyCharacter::AEnemyCharacter()
{
	//Initialise senses
	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>( TEXT( "PawnSensingComp" ) );
	pawnSensingComp->SetPeripheralVisionAngle( 90.0f );

	// Default enemy health
	m_health = 100.0f;
}

AEnemyCharacter::~AEnemyCharacter()
{
}

void AEnemyCharacter::UpdateWalkSpeed( float chaseSpeed )
{
	// Set max walk speed
	GetCharacterMovement()->MaxWalkSpeed = chaseSpeed;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( pawnSensingComp )
	{
		// If enemy 'senses' player
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
	// Get reference to player controller
	AEnemyController* enemycontroller = Cast<AEnemyController>( GetController() );

	if( enemycontroller )
	{
		// Debug message
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Pew" ) ) );

		// Set bool
		enemycontroller->SetPlayerCaught( pawn );
		
		// Shoot at player
		Shoot();
	}

	m_projectileManager.OnHit();
}