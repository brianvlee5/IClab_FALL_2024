import argparse
import math

class Rect:
    def __init__(self, instName, startX, startY, w, h, isFix):
        self.instName = instName
        self.startX = startX
        self.startY = startY
        self.w = w
        self.h = h
        self.isFix = isFix

    @property
    def endX(self):
        return self.startX + self.w

    @property
    def endY(self):
        return self.startY + self.h

    def __repr__(self):
        return f"Rect({self.instName}, {self.startX}, {self.startY}, {self.w}, {self.h}, {self.isFix})"

class PlacementRow:
    def __init__(self, startX, startY, siteWidth, siteHeight, totalNumoOfSites):
        self.startX = startX
        self.startY = startY
        self.siteWidth = siteWidth
        self.siteHeight = siteHeight
        self.totalNumOfSites = totalNumoOfSites


class Checker:
    def __init__(self, lg_file, opt_file):
        self.alpha = 0
        self.beta = 0
        self.dieOrigin = [0, 0]
        self.dieBorder = [0, 0]
        self.cells = []
        self.placementrows = []
        self.lg_file = lg_file
        self.opt_file = opt_file
        self.readLegalizePlacement()

    def run(self):
        print("[CHECKER] Start Testcase Checker")
        self.initialChecker()
        self.checkDieBoundary()
        self.checkOverlap()
        self.checkOnSite()
        print("[CHECKER] All Check Pass")

    def initialChecker(self):
        self.sortRects(self.cells)

    def readLegalizePlacement(self):
        with open(self.lg_file, 'r') as f:
            lines = f.readlines()

        for line in lines:
            value = line.strip().split(' ')

            # read parameter
            if value[0] == "Alpha":
                self.alpha = value[1]
            
            if value[0] == "Beta":
                self.beta = value[1]

            # read die info
            if value[0] == "DieSize":
                self.dieOrigin[0] = int(value[1])
                self.dieOrigin[1] = int(value[2])
                self.dieBorder[0] = int(value[3])
                self.dieBorder[1] = int(value[4])

            # read cell info
            if value[0].startswith("FF_1") or value[0].startswith("C"):
                instName, startX, startY, w, h, isFix = value
                self.cells.append(Rect(instName, int(startX), int(startY), int(w), int(h), isFix))

            # read placementrow info
            if value[0] == "PlacementRows":
                startX, startY, siteWidth, siteHeight, totalNumOfSites = int(value[1]), int(value[2]), int(value[3]), int(value[4]), int(value[5])
                self.placementrows.append(PlacementRow(startX, startY, siteWidth, siteHeight, totalNumOfSites))


        print("(alpha, beta) = " + self.alpha + " " + self.beta)
        print(self.dieOrigin[0], self.dieOrigin[1], self.dieBorder[0], self.dieBorder[1])
        print("Num of cells: ",  len(self.cells))
        print("Num of placementrows: ", len(self.placementrows))

    @staticmethod
    def sortRects(rects):
        rects.sort(key=lambda rect: (rect.startX, rect.endX, rect.startY, rect.endY))

    def checkDieBoundary(self):
        print("[CHECKER] Check Die Boundary")
        for cell in self.cells:
            if cell.startX < self.dieOrigin[0] or cell.endX > self.dieBorder[0]:
                print(f"[CHECKER] {cell.instName}: {cell.startX} {cell.endX}")
                raise AssertionError("Over x-axis Die Boundary !!")
            elif cell.startY < self.dieOrigin[1] or cell.endY > self.dieBorder[1]:
                print(f"[CHECKER] {cell.instName}: {cell.startY} {cell.endY}")
                raise AssertionError("Over y-axis Die Boundary !!")

    def checkOverlap(self):
        print("[CHECKER] Check Overlap")
        for i in range(len(self.cells) - 1):
            cell1 = self.cells[i]
            for j in range(i + 1, len(self.cells)):
                cell2 = self.cells[j]
                if cell2.startX > cell1.endX:
                    break
                if self.overlap(cell1, cell2):
                    print(f"[CHECKER] {cell1.instName} {cell2.instName}")
                    raise AssertionError(f"Overlap between {cell1.instName} and {cell2.instName} !!")
                    
    @staticmethod
    def overlap(rect1, rect2):
        rectUp = rect1 if rect1.startY > rect2.startY else rect2
        rectLow = rect2 if rect1.startY > rect2.startY else rect1
        if rectUp.startY >= rectLow.endY:
            return False
        rectLeft = rect1 if rect1.startX < rect2.startX else rect2
        rectRight = rect2 if rect1.startX < rect2.startX else rect1
        return rectRight.startX < rectLeft.endX

    def checkOnSite(self):
        print("[CHECKER] Check On Site")
        for cell in self.cells:
            isOnSite = False
            for row in self.placementrows:
                rowStartX = row.startX
                rowEndX = rowStartX + row.siteWidth * row.totalNumOfSites
                if cell.startY == row.startY:
                    if rowStartX <= cell.startX < rowEndX:
                        if math.floor((cell.startX - rowStartX) / row.siteWidth) == (cell.startX - rowStartX) / row.siteWidth:
                            isOnSite = True
                            break
                elif cell.startY < row.startY:
                    break
            if not isOnSite:
                print(f"[CHECKER] {cell.instName}")
                raise AssertionError(f"{cell.instName} Not On Site !!")

def main():
    parser = argparse.ArgumentParser(description="Checker for FFs and Gates")
    parser.add_argument('--lg', type=str, required=True, help="Filename for legalized placement information")
    parser.add_argument('--opt', type=str, required=True, help="Filename for optimized step information")
    args = parser.parse_args()

    checker = Checker(args.lg, args.opt)
    checker.run()


if __name__ == "__main__":
    main()