// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Total Towers: %d"), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Tank not found!"));
		}
	}

	int32 Index = 0;
	while (Index < TowerCount)
	{
		AActor* TowerActor = Towers[Index];
		ATower* Tower = Cast<ATower>(TowerActor);
		if (Tower && Tank)
		{
			Tower->Tank = Tank;
		}

		Index++;
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();
			TowerCount--;

			if (TowerCount <= 0)
			{
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverMessage = IsVictory ? "Victory!" : "Defeat!";
		UE_LOG(LogTemp, Display, TEXT("%s"), *GameOverMessage);

		FTimerHandle GameOverTimerHandler;
		GetWorldTimerManager().SetTimer(GameOverTimerHandler, this, &ABattleBlasterGameMode::GameOverTimeout, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::GameOverTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);

	IsVictory ? BattleBlasterGameInstance->LoadNextLevel() : BattleBlasterGameInstance->RestartCurrentLevel();
}
