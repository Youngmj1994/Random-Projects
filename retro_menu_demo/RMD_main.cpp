#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <unordered_map>

constexpr int32_t nPatch = 8;

class menuObject
{
public:
	menuObject() { sName = "root"; }
	menuObject(const std::string& name) { sName = name; }
	
	//setters
	menuObject& SetTable(int nColumns, int nRows) {	vCellTable = { nColumns,nRows }; return *this;	}
	menuObject& SetID(int32_t id) { nID = id; return *this; }

	//getters
	
	int32_t getID() { return nID; }
	std::string& getName() { return sName; }
	menuObject& Enable(bool b) { bEnabled = b; return *this; }
	bool getEnable() { return bEnabled; }
	olc::vi2d getSize() { return { int32_t(sName.size()),1 }; }//for now cells are one line strings, it could contain an icon or a progress bar or something else
	olc::vi2d& GetCursorPosition() { return vCursorPos; }
	menuObject* GetSelectedItem() { return &items[nCursorItem]; }

	bool hasChildren() { return !items.empty(); }

	//overload
	menuObject& operator[](const std::string& name)
	{
		if (itemPointer.count(name) == 0)
		{
			itemPointer[name] = items.size();
			items.push_back(menuObject(name));
		}

		return items[itemPointer[name]]; //this is to make accessing menu objects easier. instead of itmes.map[x], it can just be items[x]
	}

	//draw
	void drawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
	{
		//record the current pixel mode
		olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
		pge.SetPixelMode(olc::Pixel::MASK);

		//draw panel and border
		olc::vi2d vPatchPos = { 0,0 };
		for (vPatchPos.x = 0; vPatchPos.x < vSizeInPatches.x; vPatchPos.x++)
		{
			for (vPatchPos.y = 0; vPatchPos.y < vSizeInPatches.y; vPatchPos.y++)
			{
				//determine position in screen space
				olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

				//calculate which patch is needed
				olc::vi2d vSourcePatch = { 0,0 };
				if (vPatchPos.x > 0) vSourcePatch.x = 1;
				if (vPatchPos.x == vSizeInPatches.x - 1) vSourcePatch.x = 2;
				if (vPatchPos.y > 0) vSourcePatch.y = 1;
				if (vPatchPos.y == vSizeInPatches.y - 1) vSourcePatch.y = 2;

				//draw actual patch
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}
		}
		olc::vi2d vCell = { 0,0 };
		vPatchPos = { 1,1 };

		//work out visible items
		int32_t nTopLeftItem = nTopVisibleRow * vCellTable.x;
		int32_t nBottomRightItem = vCellTable.y * vCellTable.x + nTopLeftItem;

		//clamp to size of child item vector so we dont go out of bounds
		nBottomRightItem = min(int32_t(items.size()), nBottomRightItem);
		int32_t nVisibleItems = nBottomRightItem - nTopLeftItem;

		//draw scroll markers (if required)
		if (nTopVisibleRow > 0)
		{
			vPatchPos = { vSizeInPatches.x - 2,0 };
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
			olc::vi2d vSourcePatch = { 3,0 };
			pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
		}

		if ((nTotalRows - nTopVisibleRow) > vCellTable.y)
		{
			vPatchPos = { vSizeInPatches.x - 2, vSizeInPatches.y - 1 };
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
			olc::vi2d vSourcePatch = { 3,2 };
			pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
		}

		for (int32_t i = 0; i < nVisibleItems; i++)
		{
			//1d corrdinate in 2d space is y * width + x
			//and this is how you do it backwards. going from 2d to 1d
			vCell.x = i % vCellTable.x;
			vCell.y = i / vCellTable.x;

			// Patch location (including border offset and padding)
			vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1;
			vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;

			// Actual screen location in pixels
			olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

			// Display Item Header
			pge.DrawString(vScreenLocation, items[nTopLeftItem + i].sName, items[nTopLeftItem + i].bEnabled ? olc::WHITE : olc::DARK_GREY);

			if (items[nTopLeftItem + i].hasChildren())
			{
				//display indicator that panel has a sub panel
				vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1 + vCellSize.x;
				vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;
				olc::vi2d vSourcePatch = { 3,1 };
				vScreenLocation = vPatchPos * nPatch + vScreenOffset;
				pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
			}

		}


		// Calculate cursor position in screen space in case system draws it
		vCursorPos.x = (vCellCursor.x * (vCellSize.x + vCellPadding.x)) * nPatch + vScreenOffset.x - nPatch; //the minus and plus npatch is so the point of the cursor aligns with the text
		vCursorPos.y = ((vCellCursor.y - nTopVisibleRow) * (vCellSize.y + vCellPadding.y)) * nPatch + vScreenOffset.y + nPatch;
	}

	void Build() 
	{
		//recursively build all children, so they can determine their size, use that size to indicate cell sizes if this object contains more than one item
		for (auto& m : items)
		{
			if (m.hasChildren())
			{
				m.Build();
			}

			//longest child name determines cell width
			vCellSize.x = max(m.getSize().x, vCellSize.x);
			vCellSize.y = max(m.getSize().y, vCellSize.y);
		}

		// Adjust size of this object (in patches) if it were rendered as a panel
		vSizeInPatches.x = vCellTable.x * vCellSize.x + (vCellTable.x - 1) * vCellPadding.x + 2;
		vSizeInPatches.y = vCellTable.y * vCellSize.y + (vCellTable.y - 1) * vCellPadding.y + 2;

		//calculate how many rows this item has to hold
		nTotalRows = (items.size() / vCellTable.x) + (((items.size() % vCellTable.x) > 0) ? 1 : 0); //the mod operator is to determine if you add one more row or not
	}

	void ClampCursor()
	{
		// Find item in children
		nCursorItem = vCellCursor.y * vCellTable.x + vCellCursor.x;

		// Clamp Cursor
		if (nCursorItem >= int32_t(items.size()))
		{
			vCellCursor.y = (items.size() / vCellTable.x);
			vCellCursor.x = (items.size() % vCellTable.x) - 1;
			nCursorItem = items.size() - 1;
		}
	}

	void OnUp()
	{
		vCellCursor.y--;
		if (vCellCursor.y < 0) vCellCursor.y = 0;
		if (vCellCursor.y < nTopVisibleRow)
		{
			nTopVisibleRow--;
			if (nTopVisibleRow < 0) nTopVisibleRow = 0;
		}

		ClampCursor();
	}

	void OnDown()
	{
		vCellCursor.y++;
		if (vCellCursor.y == nTotalRows) vCellCursor.y = nTotalRows - 1;

		if (vCellCursor.y > (nTopVisibleRow + vCellTable.y - 1))
		{
			nTopVisibleRow++;
			if (nTopVisibleRow > (nTotalRows - vCellTable.y))
				nTopVisibleRow = nTotalRows - vCellTable.y;
		}

		ClampCursor();
	}

	void OnLeft() 
	{
		vCellCursor.x--;
		if (vCellCursor.x < 0) vCellCursor.x = 0;
		ClampCursor();
	}

	void OnRight()
	{
		vCellCursor.x++;
		if (vCellCursor.x == vCellTable.x) vCellCursor.x = vCellTable.x - 1;
		ClampCursor();
	}

	menuObject* OnConfirm()
	{
		// Check if selected item has children
		if (items[nCursorItem].hasChildren())
		{
			return &items[nCursorItem];
		}
		else
			return this;
	}

	
	

protected:
	int32_t nID = -1;
	olc::vi2d vCellTable = { 1, 0 };
	std::unordered_map<std::string, size_t> itemPointer;
	std::vector<menuObject> items;
	olc::vi2d vSizeInPatches = { 0, 0 };
	olc::vi2d vCellSize = { 0, 0 };
	olc::vi2d vCellPadding = { 2, 0 };
	olc::vi2d vCellCursor = { 0, 0 };
	int32_t nCursorItem = 0;
	int32_t nTopVisibleRow = 0;
	int32_t nTotalRows = 0;
	const olc::vi2d vPatchSize = { nPatch, nPatch };
	std::string sName;
	olc::vi2d vCursorPos = { 0, 0 };
	bool bEnabled = true;

private:

};

class menuManager
{
public:
	menuManager() {};
	void Open(menuObject* mo) { Close(); panels.push_back(mo); }
	void Close() { panels.clear(); }

	void OnUp() { if (!panels.empty()) panels.back()->OnUp(); }
	void OnDown() { if (!panels.empty()) panels.back()->OnDown(); }
	void OnLeft() { if (!panels.empty()) panels.back()->OnLeft(); }
	void OnRight() { if (!panels.empty()) panels.back()->OnRight(); }
	void OnBack() { if (!panels.empty()) panels.pop_back(); }

	menuObject* OnConfirm()
	{
		if (panels.empty()) return nullptr;

		menuObject* next = panels.back()->OnConfirm();
		if (next == panels.back())
		{
			if (panels.back()->GetSelectedItem()->getEnable())
				return panels.back()->GetSelectedItem();
		}
		else
		{
			if (next->getEnable())
				panels.push_back(next);
		}

		return nullptr;
	}

	void Draw(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset)
	{
		if (panels.empty()) return;

		// Draw Visible Menu System
		for (auto& p : panels)
		{
			p->drawSelf(pge, sprGFX, vScreenOffset);
			vScreenOffset += {10, -10};
		}

		// Draw Cursor
		olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
		pge.SetPixelMode(olc::Pixel::ALPHA);
		pge.DrawPartialSprite(panels.back()->GetCursorPosition(), sprGFX, olc::vi2d(4, 0) * nPatch, { nPatch * 2, nPatch * 2 });
		pge.SetPixelMode(currentPixelMode);

	}
private:
	std::list<menuObject*> panels;
};



class RetroMenuDemo : public olc::PixelGameEngine
{
	menuObject mo;
	menuManager mm;

public:
	RetroMenuDemo() {
		sAppName = "retro pop up menu";
	}

	olc::Sprite* sprGFX = nullptr;

public:
	bool OnUserCreate() override
	{
		sprGFX = new olc::Sprite("./RetroMenu.png");

		mo["main"].SetTable(1, 4);
		mo["main"]["Attack"].SetID(101);

		mo["main"]["Magic"].SetTable(1, 2);

		mo["main"]["Magic"]["White"].SetTable(3, 6);
		auto& menuWhiteMagic = mo["main"]["Magic"]["White"];
		menuWhiteMagic["Cure"].SetID(401);
		menuWhiteMagic["Cura"].SetID(402);
		menuWhiteMagic["Curaga"].SetID(403);
		menuWhiteMagic["Esuna"].SetID(404);

		mo["main"]["Magic"]["Black"].SetTable(3, 4);
		auto& menuBlackMagic = mo["main"]["Magic"]["Black"];
		menuBlackMagic["Fire"].SetID(201);
		menuBlackMagic["Fira"].SetID(202);
		menuBlackMagic["Firaga"].SetID(203);
		menuBlackMagic["Blizzard"].SetID(204);
		menuBlackMagic["Blizzara"].SetID(205).Enable(false);
		menuBlackMagic["Blizzaga"].SetID(206).Enable(false);
		menuBlackMagic["Thunder"].SetID(207);
		menuBlackMagic["Thundara"].SetID(208);
		menuBlackMagic["Thundaga"].SetID(209);
		menuBlackMagic["Quake"].SetID(210);
		menuBlackMagic["Quake2"].SetID(211);
		menuBlackMagic["Quake3"].SetID(212);
		menuBlackMagic["Bio"].SetID(213);
		menuBlackMagic["Bio1"].SetID(214);
		menuBlackMagic["Bio2"].SetID(215);
		menuBlackMagic["Demi"].SetID(216);
		menuBlackMagic["Demi1"].SetID(217);
		menuBlackMagic["Demi2"].SetID(218);

		mo["main"]["Defend"].SetID(102);

		mo["main"]["Items"].SetTable(2, 4).Enable(false);
		mo["main"]["Items"]["Potion"].SetID(301);
		mo["main"]["Items"]["Ether"].SetID(302);
		mo["main"]["Items"]["Elixir"].SetID(303);

		mo["main"]["Escape"].SetID(103);

		mo.Build();

		mm.Open(&mo["main"]);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		menuObject* command = nullptr;
		
		if (GetKey(olc::Key::M).bPressed) mm.Open(&mo["main"]);
		if (GetKey(olc::Key::UP).bPressed) mm.OnUp();
		if (GetKey(olc::Key::DOWN).bPressed) mm.OnDown();
		if (GetKey(olc::Key::LEFT).bPressed) mm.OnLeft();
		if (GetKey(olc::Key::RIGHT).bPressed) mm.OnRight();
		if (GetKey(olc::Key::SPACE).bPressed) command = mm.OnConfirm();
		if (GetKey(olc::Key::Z).bPressed)     mm.OnBack();

		

		if (command != nullptr)
		{
			sLastAction = "Selected: " + command->getName() + " ID: " + std::to_string(command->getID());
			mm.Close();
		}

		Clear(olc::BLACK);
		mm.Draw(*this, sprGFX, { 3,ScreenHeight() - 50 });
		DrawString(10, 200, sLastAction);
		return true;
	}
	 
	std::string sLastAction;
private:

};


int main()
{
	RetroMenuDemo demo;
	if (demo.Construct(720, 425, 2, 2))
		demo.Start();
}