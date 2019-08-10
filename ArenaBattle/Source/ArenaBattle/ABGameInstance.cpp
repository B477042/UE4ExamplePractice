// Fill out your copyright notice in the Description page of Project Settings.

#include "ABGameInstance.h"


UABGameInstance::UABGameInstance ()
{
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable>DT_ABCHARACTER(*CharacterDataPath);
	ABCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	//compile error
	//ABCHECK( ABCharacterTable->RowMap.Num( ) > 0);
}

void UABGameInstance::Init()
{
	Super::Init();
	//notice! erase when debug complete
	ABLOG(Warning, TEXT("Drop of Level 20 ABcharcter : %d"), GetABCharacterData(20)->DropExp);
}

FABCharacterData* UABGameInstance::GetABCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level), TEXT(""));
}