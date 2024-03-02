async function readDocument() {
  const editor = document.getElementsByClassName('cm-content')[0];

  // The editor lazily loads the content, so we need copy the content by a hacky way
  // TODO: Change this to a more reliable method
  editor.dispatchEvent(new KeyboardEvent('keydown', {
    bubbles: true, cancelable: true, key: 'a', code: 'KeyA', ctrlKey: true
  }))
  document.execCommand('copy')
  const content = await navigator.clipboard.readText();

  console.log("Read Document: ", content)
  return content
}

function writeDocument(content) {
  console.log("Write Document: ", content)

  const editor = document.getElementsByClassName('cm-content')[0];
  editor.innerText = content
}

chrome.action.onClicked.addListener(async (tab) => {
  const doc = await chrome.scripting.executeScript({
    target: {tabId: tab.id}, function: readDocument
  }).then(r => r[0].result)
  const formattedDoc = await chrome.runtime.sendNativeMessage('dev.flandia.typstfmt_ext_host', {doc})
  if (formattedDoc.error) {
    console.error(formattedDoc.error)
  } else {
    await chrome.scripting.executeScript({
      target: {tabId: tab.id}, function: writeDocument, args: [formattedDoc.doc]
    })
  }
});
