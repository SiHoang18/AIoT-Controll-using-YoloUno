import { LedWidget, RGBWidget } from "../scripts/cards.js";
import { ports } from "../scripts/settings.js";
import { sendMessage, onMessage } from "./server.js";

let boardList = [];

function getElements() {
  return {
    addBoardBtn: document.getElementById("add-board-btn"),
    modal: document.getElementById("board-modal"),
    connectBtn: document.getElementById("connect-btn"),
    closeModalBtn: document.getElementById("close-modal"),
    boardSelect: document.getElementById("app-board-select"),
    boardNameInput: document.getElementById("board-name"),
    boardTypeInput: document.getElementById("app-board-type"),
    boardMacInput: document.getElementById("board-mac"),
    deleteBoardBtn: document.getElementById("app-remove-board-btn"),
    addWidgetBtn: document.getElementById("add-widget-btn"),
    widgetModal: document.getElementById("widget-modal"),
    widgetTypeSelect: document.getElementById("widget-type"),
    widgetPortInput: document.getElementById("widget-port"),
    widgetNameInput: document.getElementById("widget-name"),
    confirmAddWidgetBtn: document.getElementById("confirm-add-widget"),
    cancelAddWidgetBtn: document.getElementById("cancel-add-widget")
  };
}

function attachEventListeners(elements) {
  elements.addBoardBtn.addEventListener("click", () => {
    elements.modal.classList.remove("hidden");
  });

  elements.deleteBoardBtn.addEventListener("click", () => {
    handleDeleteBoard(elements);
  });

  elements.closeModalBtn.addEventListener("click", () => {
    elements.modal.classList.add("hidden");
  });

  elements.connectBtn.addEventListener("click", () => {
    handleConnect(elements);
  });

  elements.addWidgetBtn.addEventListener("click", () => {
    const selectedIndex = elements.boardSelect.value;
    const selectedBoard = boardList[selectedIndex];
    const boardType = selectedBoard?.board_type || "YoloUno";
    populatePortOptions(elements, boardType);
    elements.widgetModal.classList.remove("hidden");
  });

  elements.cancelAddWidgetBtn.addEventListener("click", () => {
    elements.widgetModal.classList.add("hidden");
  });

  elements.confirmAddWidgetBtn.addEventListener("click", () => {
    addWidgetFromModal(elements);
  });

  elements.boardSelect.addEventListener("change", () => {
    const selectedIndex = elements.boardSelect.value;
    const selectedBoard = boardList[selectedIndex];
    elements.deleteBoardBtn.style.display =
      !selectedBoard || selectedBoard.name === "My Board" ? "none" : "inline-block";

    renderWidgetsForBoard(selectedBoard);
  });
}

function handleDeleteBoard(elements) {
  const selectedIndex = parseInt(elements.boardSelect.value, 10);
  const selectedBoard = boardList[selectedIndex];

  if (!selectedBoard || selectedBoard.name === "My Board") {
    alert("Cannot delete the default board.");
    return;
  }

  const confirmed = confirm(`Are you sure you want to delete board "${selectedBoard.name}"?`);
  if (!confirmed) return;

  boardList.splice(selectedIndex, 1);
  updateBoardSelect(elements.boardSelect);
  triggerSelectChange(elements);
  syncBoardListToBackend();
}

function populatePortOptions(elements, boardType = "YoloUno") {
  const portSelect = elements.widgetPortInput;
  portSelect.innerHTML = "";

  const portList = ports[boardType] || [];

  const defaultOption = document.createElement("option");
  defaultOption.disabled = true;
  defaultOption.selected = true;
  defaultOption.textContent = "Select port";
  portSelect.appendChild(defaultOption);

  portList.forEach(port => {
    const option = document.createElement("option");
    option.value = port;
    option.textContent = port;
    portSelect.appendChild(option);
  });
}

function renderWidgetsForBoard(board) {
  const container = document.querySelector(".content-container");
  container.innerHTML = '';

  board.widgets.forEach(widgetData => {
    let widget;
    switch (widgetData.type) {
      case "LED":
        widget = new LedWidget("LED", container, widgetData.name, widgetData.port, board.mac_address);
        break;
      case "LED_RGB":
        widget = new RGBWidget("LED_RGB", container, widgetData.name, widgetData.port, board.mac_address);
        break;
      default:
        console.warn("Unknown widget type:", widgetData.type);
    }
  });
}

function addWidgetFromModal(elements) {
  const name = elements.widgetNameInput?.value.trim();
  const port = parseInt(elements.widgetPortInput?.value, 10);
  const type = elements.widgetTypeSelect?.value;

  const container = document.querySelector(".content-container");
  const selectedIndex = elements.boardSelect.value;
  const selectedBoard = boardList[selectedIndex];
  const mac = selectedBoard.mac_address;

  if (!name || isNaN(port) || !type) {
    alert("Please fill out all widget fields.");
    return;
  }

  const isDuplicate = selectedBoard.widgets.some(
    widget => widget.type === type || widget.port === port
  );

  if (isDuplicate) {
    alert("Widget existed!");
    return;
  }

  let widget;
  switch (type) {
    case "LED":
      widget = new LedWidget("LED", container, name, port, mac);
      break;
    case "LED_RGB":
      widget = new RGBWidget("LED_RGB", container, name, port, mac);
      break;
    default:
      alert("Unsupported widget type.");
      return;
  }

  selectedBoard.widgets.push({ type, name, port });
  elements.widgetModal.classList.add("hidden");

  syncBoardListToBackend();
}

async function fetchBoardsFromStorage(elements) {
  try {
    const response = await fetch("/boards.dat");
    if (!response.ok) throw new Error("Failed to load board data.");

    const jsonData = await response.json();
    if (!Array.isArray(jsonData) || jsonData.length === 0) {
      boardList = [{
        name: "My Board",
        board_type: "YoloUno",
        mac_address: "",
        widgets: []
      }];
    } else {
      boardList = jsonData;
      if (!boardList.some(board => board.name === "My Board")) {
        boardList.unshift({
          name: "My Board",
          board_type: "YoloUno",
          mac_address: "",
          widgets: []
        });
      }
    }
    syncBoardListToBackend();
    updateBoardSelect(elements.boardSelect);
    triggerSelectChange(elements);

  } catch (err) {
    console.error("Error loading board data:", err);
    boardList = [{
      name: "My Board",
      board_type: "YoloUno",
      mac_address: "",
      widgets: []
    }];
    syncBoardListToBackend();
    updateBoardSelect(elements.boardSelect);
    triggerSelectChange(elements);
  }
}

function updateBoardSelect(boardSelect) {
  boardSelect.innerHTML = '';
  boardList.forEach((board, idx) => {
    const option = document.createElement("option");
    option.value = idx;
    option.textContent = board.name;
    if (board.name === "My Board") option.selected = true;
    boardSelect.appendChild(option);
  });
}

function handleConnect(elements) {
  const name = elements.boardNameInput.value.trim();
  const type = elements.boardTypeInput.value.trim();
  const mac = elements.boardMacInput.value.trim();
  if (!name || !mac || !type) {
    alert("Please enter both name and MAC address.");
    return;
  }
  const newBoard = {
    name: name,
    board_type: type,
    mac_address: mac,
    widgets: []
  };
  // sendMessage({
  //   title: "add-board",
  //   data: newBoard
  // });
  // onMessage((data) => {
  //   console.log('Received :', data);
  //   if(data.status === "error"){
  //     console.log("Fail to connect");
  //     return;
  //   }
  //   else{
  //     console.log("add success");
  //   }
  // });

  boardList.push(newBoard);
  updateBoardSelect(elements.boardSelect);
  elements.modal.classList.add("hidden");
  triggerSelectChange(elements);
  syncBoardListToBackend();
}

function triggerSelectChange(elements) {
  const event = new Event("change");
  elements.boardSelect.dispatchEvent(event);
}

function syncBoardListToBackend() {
  sendMessage({
    title: "update-board",
    data: boardList
  });
}

function initialize() {
  const elements = getElements();
  attachEventListeners(elements);
  fetchBoardsFromStorage(elements);
}

document.addEventListener("DOMContentLoaded", initialize);
