// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Misc/App.h"
#include "EnemyController.h"



APlayerCharacter::APlayerCharacter()
	:m_invisible( false )
	,m_invisibilityPercent( 100.0f )
{
	
	
}

APlayerCharacter::~APlayerCharacter()
{
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer( m_invisibilityTimer, this, &APlayerCharacter::InvisibilityTimer, 1.0f, true, 2.0f );
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

void APlayerCharacter::SetInvisible( bool invisible )
{
	m_invisible = invisible;
}

bool APlayerCharacter::GetInvisible() const
{
	return m_invisible;
}


void APlayerCharacter::SetupPlayerInputComponent( UInputComponent* playerInputComponent )
{
	Super::SetupPlayerInputComponent( playerInputComponent );

	playerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &APlayerCharacter::TakedownTrace );

	playerInputComponent->BindAction( "PowerUp", IE_Pressed, this, &APlayerCharacter::Invisibility );
}




void APlayerCharacter::Invisibility()
{
	
	if( m_invisibilityPercent > 0.0f )
	{
		m_invisible = !m_invisible;
	}

	// Debug
	if( m_invisible )
	{
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, ( TEXT( "INVISIBILITY ON" ) ) );
	}
	else
	{
		GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, ( TEXT( "INVISIBILITY OFF" ) ) );
	}
	
}

void APlayerCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FString TheFloatStr = FString::SanitizeFloat( m_invisibilityPercent );
	GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Blue, ( *TheFloatStr ) );

	if( m_invisibilityPercent > 0.0f && m_invisible == true )
	{
		m_invisibilityPercent--;
	}

	if( m_invisibilityPercent < 100.0f && m_invisible == false )
	{
		m_invisibilityPercent++;
	}
}

void APlayerCharacter::InvisibilityTimer()
{
	GetWorldTimerManager().ClearTimer( m_invisibilityTimer );

	

	
}
