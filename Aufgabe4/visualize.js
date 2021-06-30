// alert("Test!")

var hoelzer = []
var ziel = []
var add = []
var rem = []

var canvas = document.getElementById("myCanvas")

var fin = document.getElementById("fin")
fin.onchange = function(event) {readFile(fin, hoelzer, ziel)}

var floes = document.getElementById("floes")
floes.onchange = function(event) {readFile(floes, rem, add)}

var save = document.getElementById("save")
save.onclick = function(event) {
    let link = document.getElementById("link")
    link.setAttribute("download", "streichholz.png")
    link.setAttribute("href", canvas.toDataURL("image/png").replace("image/png", "image/octet-stream"))
    link.click()
}

function readFile(input, list1, list2)
{
    var file = input.files[0]
    var reader = new FileReader()
    reader.readAsText(file)
    reader.onload = function()
    {
        list1.length = 0
        list2.length = 0
        let lines = reader.result.split("\n")
        let len = parseInt(lines[0])
        for(let i = 1; i < len + 1; ++i)
        {
            let nums = lines[i].split(" ")
            list1.push({x: {a: parseInt(nums[0]), b: parseInt(nums[1])},
                          y: {a: parseInt(nums[2]), b: parseInt(nums[3])},
                          dir: parseInt(nums[4])})
        }
        for(let i = len+2; i < 2*len+2; ++i)
        {
            let nums = lines[i].split(" ")
            list2.push({x: {a: parseInt(nums[0]), b: parseInt(nums[1])},
                          y: {a: parseInt(nums[2]), b: parseInt(nums[3])},
                          dir: parseInt(nums[4])})
        }
        draw()
        console.log(hoelzer)
    }
}

const bfactor = Math.sqrt(3) / 2
var holzlen = 80

function drawHoelzer(ctx, h)
{
    h.forEach(holz => {
        let x1 = holz.x.a / 2 + holz.x.b * bfactor
        let y1 = holz.y.a / 2 + holz.y.b * bfactor
        let x2 = x1 + Math.cos(Math.PI / 6 * holz.dir)
        let y2 = y1 + Math.sin(Math.PI / 6 * holz.dir)
        ctx.beginPath()
        ctx.moveTo(x1 * holzlen, -y1 * holzlen)
        ctx.lineTo(x2 * holzlen, -y2 * holzlen)
        ctx.stroke()
    })
}

function draw()
{
    var ctx = canvas.getContext("2d")
    ctx.clearRect(0, 0, 500, 500)
    ctx.translate(250, 250)
    ctx.lineWidth = 10
    ctx.strokeStyle = "#FFAA00"
    drawHoelzer(ctx, hoelzer)
    ctx.strokeStyle = "#FF0000"
    drawHoelzer(ctx, rem)
    ctx.strokeStyle = "#00FF00"
    drawHoelzer(ctx, add)
    ctx.strokeStyle = "#FFAA00"
    ctx.translate(400, 0)
    drawHoelzer(ctx, ziel)
    ctx.translate(-650, -250)
}

