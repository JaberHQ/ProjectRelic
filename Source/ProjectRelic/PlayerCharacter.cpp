// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnemyController.h"

APlayerCharacter::APlayerCharacter()

{
}

APlayerCharacter::~APlayerCharacter()
{
}

void APlayerCharacter::TakedownTrace()
{
	//// Line tracing
	//FVector Loc;
	//FRotator Rot;
	//FHitResult Hit;

	//GetController()->GetPlayerViewPoint( Loc, Rot );
	//FVector Start = Loc;
	//FVector End = Start + ( Rot.Vector() * 2000.0f );

	//FCollisionQueryParams TraceParams;
	//bool bHit = GetWorld()->LineTraceSingleByChannel( Hit, Start, End, ECC_Camera, TraceParams );

	//DrawDebugLine( GetWorld(), Start, End, FColor::Red, false, 2.0f );

	//if( Hit.GetActor() != NULL )
	//{
	//	float m_dotProduct = FVector::DotProduct( Hit.GetActor()->GetActorForwardVector(), GetActorForwardVector() );

	//	// A = dot product
	//	float B = 1.0f; // Close to 1
	//	float m_errorTolerance = 0.1f; // Error tolerance

	//	if( FMath::IsNearlyEqual( m_dotProduct, B, m_errorTolerance ) )
	//	{
	//		
	//		ICombatInterface* Interface = Cast<ICombatInterface>( Hit.GetActor() );
	//		if( Interface )
	//		{
	//			if( Interface->CanTakedown() == true )
	//			{
	//				// Debug
	//				GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, Hit.GetActor()->GetName() );
	//				AEnemyController* enemyController = Cast<AEnemyController>( GetController() );


	//				FVector playerLocation = Hit.GetActor()->GetActorLocation() + ( Hit.GetActor()->GetActorForwardVector() * -50.0f );
	//				
	//				PossessedBy( enemyController );
	//				//enemyController->MoveToLocation( playerLocation );

	//				//AEnemyCharacter* enemyCharacter = Cast<AEnemyCharacter>( pawn );

	//				//enemyCharacter->UnPossessed();


	//			}
	//		}
	//	}
	//}
}

void APlayerCharacter::Takedown()
{
	//AActor Target = GetActorLocation() + ( GetActorForwardVector() * -50.0f );
	
}

void APlayerCharacter::PrepareTakedown()
{
}


void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* playerInputComponent )
{
	Super::SetupPlayerInputComponent( playerInputComponent );

	playerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &APlayerCharacter::TakedownTrace );
}


