#pragma once

#include "CoreMinimal.h"

class UOWPathAsset;

namespace OWPathAssetEdMode {
	DECLARE_DELEGATE_OneParam(FOWPathAssetChangedDelegate, TWeakObjectPtr<UOWPathAsset>);
}